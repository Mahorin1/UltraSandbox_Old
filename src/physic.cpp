//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #include <physic.hpp>
    using namespace sf;
    using namespace sw;
    using namespace std;
    class ContactListener;
    class ContactFilter;
    b2World* PhysicWorld = nullptr;
    ContactListener* CL = nullptr;
    ContactFilter* CF = nullptr;
    b2Body* JointBody = nullptr;
    int32 Vel_Iteraion; int32 Pos_Iteraion;
    float Accumulator; float WorldStep;
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2PolygonShape NewRectShape(Vector2f dims)
    { b2PolygonShape sh; sh.SetAsBox(dims.x / B2SCALE / 2, dims.y / B2SCALE / 2); return sh; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2PolygonShape NewBoxShape(float side) { return NewRectShape(Vector2f(side, side)); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2CircleShape NewCircShape(float radius)
    { b2CircleShape sh; sh.m_radius = radius / B2SCALE; return sh; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Collider* NewRectCollider(Vector2f dims, Vector2f pos, b2BodyType type) {
        b2PolygonShape shape = NewRectShape(dims);
        Collider* c = new Collider(shape, pos, type); return c;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Collider* NewBoxCollider(float side, Vector2f pos, b2BodyType type) {
        b2PolygonShape shape = NewBoxShape(side);
        Collider* c = new Collider(shape, pos, type); return c;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Collider* NewCircCollider(float radius, Vector2f pos, b2BodyType type) {
        b2CircleShape shape = NewCircShape(radius);
        Collider* c = new Collider(shape, pos, type); return c;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    ShapedCollider* NewRect(Vector2f dims, Vector2f pos, b2BodyType type) {
        b2PolygonShape shape = NewRectShape(dims);
        ShapedCollider* sc = new ShapedCollider(shape, pos, type); return sc;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//    
    ShapedCollider* NewBox(float side, Vector2f pos, b2BodyType type) {
        b2PolygonShape shape = NewBoxShape(side);
        ShapedCollider* sc = new ShapedCollider(shape, pos, type); return sc;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    ShapedCollider* NewCirc(float radius, Vector2f pos, b2BodyType type) {
        b2CircleShape shape = NewCircShape(radius);
        ShapedCollider* sc = new ShapedCollider(shape, pos, type); return sc;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Collider::Collider(b2Shape& shape, Vector2f pos, b2BodyType type) {
        b2BodyDef bodyDef;
        bodyDef.type = type;
        bodyDef.position = sf_to_b2(pos);
        bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
        m_body = PhysicWorld->CreateBody(&bodyDef);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 1;
        m_fixture = m_body->CreateFixture(&fixtureDef);
        Begin_CB = nullptr; End_CB = nullptr;
        Pre_CB = nullptr; Post_CB = nullptr;
        m_userDataPtr = 0;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Collider::destroy(Collider* &ptr) {
        m_body->DestroyFixture(m_fixture);
        PhysicWorld->DestroyBody(m_body);
        m_fixture = nullptr; m_body = nullptr;
        ptr = nullptr; delete this;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    bool Collider::testPoint(Vector2f pos) {
        b2Vec2 b2Pos = sf_to_b2(pos);
        for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
        { if (f->TestPoint(b2Pos)) { return true; } }
        return false;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Collider::setSensor(bool b) { m_fixture->SetSensor(b); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Collider::setGravityScale(float g) { m_body->SetGravityScale(g); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Collider::setUserData(uintptr_t ptr) { m_userDataPtr = ptr; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Collider::setAngle(float a, bool deg)
    { a = (deg) ? toRad(a) : a; m_body->SetTransform(m_body->GetPosition(), a); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Collider::setPos(Vector2f pos) { m_body->SetTransform(sf_to_b2(pos), m_body->GetAngle()); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Collider::setType(b2BodyType t) { m_body->SetType(t); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    bool Collider::isSensor() { return m_fixture->IsSensor(); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Collider::getAngle(bool deg) { return (deg) ? toDeg(m_body->GetAngle()) : m_body->GetAngle(); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    uintptr_t Collider::getUserData() { return m_userDataPtr; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f Collider::getPos() { return b2_to_sf(m_body->GetPosition()); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Collider::getTotalVel() { return m_body->GetLinearVelocity().Length() * B2SCALE; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Collider::getDensity() { return m_fixture->GetDensity(); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2Body* Collider::getBody() { return m_body; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2BodyType Collider::getType() { return m_body->GetType(); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2Fixture* Collider::getFixture() { return m_fixture; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    ShapedCollider::ShapedCollider(b2Shape& shape, Vector2f pos, b2BodyType type)
    : Collider(shape, pos, type) {
    //-------------//
        if (shape.m_type == 0) {
            float r = shape.m_radius * B2SCALE;
            m_sfShape = new CircleShape(r);
            m_sfShape->setOrigin(r, r);
        } else if (shape.m_type == 2) {
            const b2PolygonShape& b2polyShape = static_cast<const b2PolygonShape&>(shape);
            int nbVertices = b2polyShape.m_count;
            ConvexShape convexTemp(nbVertices);
            for (int i = 0; i < nbVertices; ++i) {
                b2Vec2 vertex = b2polyShape.m_vertices[i];
                convexTemp.setPoint(i, b2_to_sf(vertex));
            }
            FloatRect bounds = convexTemp.getLocalBounds();
            float cx = bounds.left + bounds.width / 2.f;
            float cy = bounds.top + bounds.height / 2.f;
            convexTemp.setOrigin(cx, cy);
            m_sfShape = new ConvexShape(convexTemp);
        } m_sfShape->setPosition(pos);
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Shape* ShapedCollider::getShape() { return m_sfShape; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void ShapedCollider::destroy(ShapedCollider* &ptr) {
        delete m_sfShape; m_sfShape = nullptr;
        Collider::destroy((Collider*&)ptr);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void ShapedCollider::update() {
        m_sfShape->setPosition(getPos());
        m_sfShape->setRotation(getAngle());
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void ShapedCollider::draw(RenderWindow& window)
    { if (m_sfShape) { window.draw(*m_sfShape); } }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    DragJoint::DragJoint(float RayonA, float RayonB, float LineWidth)
    : m_angleActive(false), m_targetBody(nullptr), m_joint(nullptr) {
    //-------------//
        m_lineWidth = LineWidth;
        m_shapeA.setRadius(RayonA);
        m_shapeB.setRadius(RayonB);
        m_shapeA.setOutlineThickness(2.f);
        m_shapeB.setOutlineThickness(2.f);
        m_shapeA.setFillColor(Color::Yellow);
        m_shapeB.setFillColor(Color::Cyan);
        m_shapeA.setOutlineColor(Color::Black);
        m_shapeB.setOutlineColor(Color::Black);
        m_shapeA.setOrigin(4.5f, 4.5f);
        m_shapeB.setOrigin(4.5f, 4.5f);
        Color rect_col(250,250,250,200);
        m_rectLine.setFillColor(rect_col);
        m_rectLine.setOrigin(Vector2f(0, LineWidth/2));
        m_rectLine.setOutlineColor(Color::Black);
        m_rectLine.setOutlineThickness(2.f);
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DragJoint::unSet() {
        setAngleMode(false);
        PhysicWorld->DestroyJoint(m_joint);
        m_targetBody = nullptr;
        m_joint = nullptr;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DragJoint::setObj(b2Body* body, Vector2f pos) {
        if (isActive()) { unSet(); }
        m_angleActive = false; m_targetBody = body;
        m_joint = NewMouseJoint(body, pos, 17.f, 2.5f, 3000.f);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    bool DragJoint::isActive() { return m_joint; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    bool DragJoint::sameBody(b2Body* b) { return (b == m_targetBody); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DragJoint::setAngleMode(bool b) {
        if (isActive()) {
            if (b && !m_angleActive) {
                m_saveDamping = m_targetBody->GetAngularDamping();
                m_saveFixedRot = m_targetBody->IsFixedRotation();
                m_targetBody->SetAngularDamping(30.f);
                m_saveMassData = m_targetBody->GetMassData();
                auto data = m_saveMassData;
                data.mass = 1.f; data.I = 1.f;
                m_targetBody->SetFixedRotation(true);
                m_targetBody->SetMassData(&data);
            } else if (!b && m_angleActive) {
                m_targetBody->SetAngularDamping(m_saveDamping);
                m_targetBody->SetFixedRotation(m_saveFixedRot);
                m_targetBody->SetMassData(&m_saveMassData);
            } m_angleActive = b;
        }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    bool DragJoint::getAngleMode() { return m_angleActive; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DragJoint::rotate(float vel)
    { if (m_angleActive && isActive()) { m_targetBody->SetAngularVelocity(vel); } }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2Body* DragJoint::getBody() { return m_targetBody; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f DragJoint::getAnchor(bool first)
    { return ((first) ? b2_to_sf(m_joint->GetAnchorA()) : b2_to_sf(m_joint->GetAnchorB())); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DragJoint::update() {
        if (isActive()) {
            Vector2f posA = getAnchor();
            Vector2f posB = getAnchor(false);
            m_shapeA.setPosition(posA);
            m_shapeB.setPosition(posB);
            float a = Math_Angle(posA, posB);
            float d = Math_Dist(posA, posB);
            m_rectLine.setRotation(toDeg(a));
            m_rectLine.setSize(Vector2f(d, m_lineWidth));
            m_rectLine.setPosition(posA);
        }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DragJoint::target(Vector2f pos)
    { if (isActive()) { m_joint->SetTarget(sf_to_b2(pos)); } }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DragJoint::destroy() { unSet(); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DragJoint::draw(RenderWindow& w) {
        if (isActive()) {
            w.draw(m_rectLine);
            w.draw(m_shapeB);
            w.draw(m_shapeA);
        }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Collider* getObj(b2Fixture* f) {
        uintptr_t ptr = f->GetBody()->GetUserData().pointer;
        return reinterpret_cast<Collider*>(ptr);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    bool SameUserData(Collider* a, Collider* b) {
        uintptr_t ptrA = a->getUserData();
        uintptr_t ptrB = b->getUserData();
        if (!(ptrA==0) && (ptrA==ptrB))
        { return false; } return true;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class ContactListener : public b2ContactListener {
    //-------------//
        void BeginContact(b2Contact* c) {
            Collider* ObjA = getObj(c->GetFixtureA());
            Collider* ObjB = getObj(c->GetFixtureB());
            if (ObjA->Begin_CB) { ObjA->Begin_CB(ObjB, c); };
            if (ObjB->Begin_CB) { ObjB->Begin_CB(ObjA, c); };
        }
    //-------------//
        void EndContact(b2Contact* c) {
            Collider* ObjA = getObj(c->GetFixtureA());
            Collider* ObjB = getObj(c->GetFixtureB());
            if (ObjA->End_CB) { ObjA->End_CB(ObjB, c); };
            if (ObjB->End_CB) { ObjB->End_CB(ObjA, c); };
        }
    //-------------//
        void PreSolve(b2Contact* c, const b2Manifold* oldManifold) {
            Collider* ObjA = getObj(c->GetFixtureA());
            Collider* ObjB = getObj(c->GetFixtureB());
            if (ObjA->Pre_CB) { ObjA->Pre_CB(ObjB, c); };
            if (ObjB->Pre_CB) { ObjB->Pre_CB(ObjA, c); };
        }
    //-------------//
        void PostSolve(b2Contact* c, const b2ContactImpulse* impulse) {
            Collider* ObjA = getObj(c->GetFixtureA());
            Collider* ObjB = getObj(c->GetFixtureB());
            if (ObjA->Post_CB) { ObjA->Post_CB(ObjB, c); };
            if (ObjB->Post_CB) { ObjB->Post_CB(ObjA, c); };
        }
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class ContactFilter : public b2ContactFilter {
    //-------------//
        bool ShouldCollide(b2Fixture* f1, b2Fixture* f2) {
        //-------------//
            Collider* c1 = getObj(f1);
            Collider* c2 = getObj(f2);
            return SameUserData(c1, c2);
        //-------------//
        }
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void InitWorld(float gx, float gy, float step, int32 vel_iter, int32 pos_iter) {
        if (PhysicWorld == nullptr) {
            b2Vec2 gravity(gx, gy);
            PhysicWorld = new b2World(gravity);
            CL = new ContactListener();
            CF = new ContactFilter();
            PhysicWorld->SetContactListener(CL);
            PhysicWorld->SetContactFilter(CF);
            Accumulator = 0.f;
            SetWorldStep(step, vel_iter, pos_iter);
            b2BodyDef b_def;
            JointBody = PhysicWorld->CreateBody(&b_def);
        }
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void SetWorldStep(float step, int32 vel_iter, int32 pos_iter)
    { WorldStep = step; Vel_Iteraion = vel_iter; Pos_Iteraion = pos_iter; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void UpdateWorld(float dt, float speed) {
    //-------------//
        while (Accumulator >= WorldStep) {
            PhysicWorld->Step(WorldStep * speed, Vel_Iteraion, Pos_Iteraion);
            Accumulator -= WorldStep;
        } Accumulator += dt;
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class PhysicQueryCallback : public b2QueryCallback {
    //-------------//
    public:
        PhysicQueryCallback(FloatRect& z, UserQuery_CB user_cb)
        : m_zone(z), m_userCB(user_cb) {}
        bool ReportFixture(b2Fixture* f)
        { return m_userCB(m_zone, getObj(f), f); }
    //-------------//
    private:
        FloatRect m_zone;
        UserQuery_CB m_userCB;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class PhysicRayCastCallback : public b2RayCastCallback {
    //-------------//
    public:
        PhysicRayCastCallback(Vector2f p1, Vector2f p2, UserRayCast_CB user_cb)
        : m_startP(p1), m_endP(p2), m_userCB(user_cb) {}
    //-------------//
        float ReportFixture(b2Fixture* f, const b2Vec2& p, const b2Vec2& n, float fraction) {
            RayHit h(f, p, n, fraction, m_startP, m_endP);
            return m_userCB(getObj(f), h);
        }
    //-------------//
    private:
        Vector2f m_startP;
        Vector2f m_endP;
        UserRayCast_CB m_userCB;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void QueryWorldZone(FloatRect& zone, UserQuery_CB user_cb) {
        PhysicQueryCallback cb(zone, user_cb);
        PhysicWorld->QueryAABB(&cb, sf_to_b2(zone));
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void WorldRayCast(Vector2f p1, Vector2f p2, UserRayCast_CB user_cb) {
        PhysicRayCastCallback cb(p1, p2, user_cb);
        PhysicWorld->RayCast(&cb, sf_to_b2(p1), sf_to_b2(p2));
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2RevoluteJoint* NewBearing(b2Body* b1, b2Body* b2, Vector2f p, float ao, bool cc) {
        b2RevoluteJointDef jdef; jdef.collideConnected = cc;
        jdef.Initialize(b1, b2, sf_to_b2(p)); jdef.referenceAngle = ao;
        return (b2RevoluteJoint*)PhysicWorld->CreateJoint(&jdef);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2RevoluteJoint* NewBearing(b2Body* b, Vector2f p, float ao) {
        b2RevoluteJointDef jdef;
        jdef.Initialize(JointBody, b, sf_to_b2(p));
        jdef.referenceAngle = ao;
        return (b2RevoluteJoint*)PhysicWorld->CreateJoint(&jdef);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2DistanceJoint* NewLink(b2Body* b1, b2Body* b2, Vector2f p1, Vector2f p2, float h, float d, bool cc) {
        b2DistanceJointDef jdef; jdef.collideConnected = cc;
        jdef.Initialize(b1, b2, sf_to_b2(p1), sf_to_b2(p2));
        if (!(h == -1.f && d == -1.f)) {
            b2LinearStiffness(jdef.stiffness, jdef.damping, h, d, b1, b2);
        } return (b2DistanceJoint*)PhysicWorld->CreateJoint(&jdef);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2DistanceJoint* NewLink(b2Body* b1, b2Body* b2, float h, float d, bool cc) {
        b2DistanceJointDef jdef; jdef.collideConnected = cc;
        jdef.Initialize(b1, b2, b1->GetPosition(), b2->GetPosition());
        if (!(h == -1.f && d == -1.f)) {
            b2LinearStiffness(jdef.stiffness, jdef.damping, h, d, b1, b2);
        } return (b2DistanceJoint*)PhysicWorld->CreateJoint(&jdef);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2DistanceJoint* NewLink(b2Body* b, Vector2f p1, Vector2f p2, float h, float d) {
        b2DistanceJointDef jdef;
        jdef.Initialize(JointBody, b, sf_to_b2(p1), sf_to_b2(p2));
        if (!(h == -1.f && d == -1.f)) {
            b2LinearStiffness(jdef.stiffness, jdef.damping, h, d, JointBody, b);
        } return (b2DistanceJoint*)PhysicWorld->CreateJoint(&jdef);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2MouseJoint* NewMouseJoint(b2Body* b, sf::Vector2f pos, float h, float d, float f) {
        b2MouseJointDef jdef; jdef.bodyA = JointBody; jdef.bodyB = b;
        jdef.maxForce = f * b->GetMass(); jdef.target = sf_to_b2(pos);
        b2LinearStiffness(jdef.stiffness, jdef.damping, h, d, JointBody, b);
        
        return (b2MouseJoint*)PhysicWorld->CreateJoint(&jdef);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2WheelJoint* NewWheelJoint(b2Body* b1, b2Body* b2, Vector2f anchor, b2Vec2 axis, bool cc) {
        b2WheelJointDef jdef; jdef.collideConnected = cc;
        jdef.Initialize(b1, b2, sf_to_b2(anchor), axis);
        return (b2WheelJoint*)PhysicWorld->CreateJoint(&jdef);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2WeldJoint* NewWeldJoint(b2Body* b1, b2Body* b2, Vector2f p) {
        b2WeldJointDef jdef; jdef.collideConnected = false;
        jdef.Initialize(b1, b2, sf_to_b2(p));
        return (b2WeldJoint*)PhysicWorld->CreateJoint(&jdef);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2Body* NewBody(Vector2f p, float m, float i) {
        b2BodyDef bdef; bdef.type = b2_dynamicBody;
        b2MassData md; bdef.position = sf_to_b2(p);
        auto* b = PhysicWorld->CreateBody(&bdef);
        md.center = b2Vec2_zero; md.mass = m;
        md.I = i; b->SetMassData(&md); return b;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2Vec2 GetWorldCentreOfMass(std::vector<b2Body*>& bodyLst) {
    //-------------//
        float totalMass = 0.f;
        for (auto& b : bodyLst)
        { totalMass += b->GetMass(); }
    //-------------//
        b2Vec2 centreOfMass(0.f, 0.f);
        for (auto& b : bodyLst) {
            b2Vec2 pos = b->GetWorldCenter();
            float m = b->GetMass();
            pos.x *= m / totalMass;
            pos.y *= m / totalMass;
            centreOfMass += pos;
        } return centreOfMass;
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    StaticRope::StaticRope(b2Body* b1, b2Body* b2, Vector2f p1, Vector2f p2, int seg, float l)
    : m_segments(seg), m_lastMode(0) {
    //-------------//
        m_rope = NewLink(b1, b2, p1, p2);
        m_rope->SetMinLength(0.f);
        if (l != -1) { setLength(l); }
        else { m_length = b2_to_sf(m_rope->GetLength()); }
        m_curve.setIterations(80);
        m_spline.setThickness(2.5f);
        m_spline.setColor(Color::Black);
        m_spline.setThickCornerType(Spline::ThickCornerType::Round);
        m_spline.setThickStartCapType(Spline::ThickCapType::Round);
        m_spline.setThickEndCapType(Spline::ThickCapType::Round);
        m_spline.addVertex(); compute(p1, p2);
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void StaticRope::compute(Vector2f p1, Vector2f p2) {
    //-------------//
        m_curve.Solve(p1, p2, m_length);
        auto mode = m_curve.getMode();
        bool is_different = ( mode != m_lastMode );
    //-------------//
        switch (mode) {
        //-------------//
            case 1:
                if (is_different) {
                    m_spline.removeVertices(0);
                    m_spline.addVertices(2);
                } m_spline.setPosition(0, p1);
                m_spline.setPosition(1, p2);
                break;
        //-------------//
            case 2:
                if (is_different) {
                    m_spline.removeVertices(0);
                    m_spline.addVertices(3);
                }
                {const auto midp = m_curve.getMidP();
                m_spline.setPosition(0, p1);
                m_spline.setPosition(1, midp);
                m_spline.setPosition(2, p2);}
                break;
        //-------------//
            case 3:
                if (is_different) {
                    m_spline.removeVertices(0);
                    m_spline.addVertices(m_segments+1);
                } m_spline.setPosition(0, p1);
                m_spline.setPosition(m_segments, p2);
                {const float dx = p2.x - p1.x;
                for (size_t i = 1; i < m_segments; i++) {
                    float t = (float)(i) / (float)(m_segments);
                    Vector2f p(p1.x + t * dx, 0);
                    p.y = m_curve.getY(p.x);
                    m_spline.setPosition(i, p);
                } } break;
        //-------------//
        } m_lastMode = mode;
        m_spline.update();
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void StaticRope::update() {
    //-------------//
        Vector2f p1 = getAnchor();
        Vector2f p2 = getAnchor(false);
        if (p1 != m_oldP1 || p2 != m_oldP2)
        { compute(p1, p2); } m_oldP1 = p1, m_oldP2 = p2;
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void StaticRope::draw(RenderWindow& w) { w.draw(m_spline); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float StaticRope::getLength() { return m_length; }
    Spline* StaticRope::getSpline() { return &m_spline; }
    b2DistanceJoint* StaticRope::getJoint() { return m_rope; }
    Vector2f StaticRope::getPoint(float t) { return m_curve.getPoint(t); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f StaticRope::getAnchor(bool first)
    { return ((first) ? b2_to_sf(m_rope->GetAnchorA()) : b2_to_sf(m_rope->GetAnchorB())); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void StaticRope::setLength(float l)
    { m_length = l; m_rope->SetLength(sf_to_b2(l)); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void StaticRope::destroy()
    { PhysicWorld->DestroyJoint(m_rope); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    DynamicRope::DynamicRope(b2Body* b1, b2Body* b2, Vector2f p1, Vector2f p2, int seg)
    : m_segments(seg) {
    //-------------//
        m_spline.addVertices(seg+1);
        m_spline.setThickness(3.f);
        m_spline.setThickCornerType(Spline::ThickCornerType::Round);
        m_spline.setThickStartCapType(Spline::ThickCapType::Round);
        m_spline.setThickEndCapType(Spline::ThickCapType::Round);
        m_spline.setBezierInterpolation(true);
        m_spline.setInterpolationSteps(100);
        auto col = RdmColor();
        for (size_t i = 0; i <= seg; i++) {
            m_spline.setColor(i, col);
            if (CoinFlip()) { col = RdmColor(); }
        }
    //-------------//
        m_spline2.addVertices(seg+1);
        m_spline2.setThickness(8.f);
        m_spline2.setColor(Color::Black);
        m_spline2.setThickCornerType(Spline::ThickCornerType::Round);
        m_spline2.setThickStartCapType(Spline::ThickCapType::Round);
        m_spline2.setThickEndCapType(Spline::ThickCapType::Round);
        m_spline2.setBezierInterpolation(true);
        m_spline2.setInterpolationSteps(100);
    //-------------//
        m_lstB = new b2Body*[seg+1]; m_lstB[0] = b1;
        auto d = p2 - p1, p = ZeroVec2f;
        for (size_t i = 1; i < seg; i++) {
            float t = (float)i / (float)seg;
            p = p1 + d * t; auto* b = NewBody(p, 0.01f);
            auto* j = (i==1) ? NewLink(b1, b, p1, p) : NewLink(m_lstB[i-1], b);
            b->SetGravityScale(2.f); j->SetMinLength(0.1f); m_lstB[i] = b;
            b->SetLinearDamping(0.8f);
        } m_lstB[seg] = b2; NewLink(m_lstB[seg-1], b2, p, p2);
        auto* j = NewLink(b1, b2, p1, p2); j->SetMinLength(0.f);
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DynamicRope::update() {
    //-------------//
        for (size_t i = 0; i <= m_segments; i++) {
            auto p = b2_to_sf(m_lstB[i]->GetPosition());
            m_spline.setPosition(i, p);
            m_spline2.setPosition(i, p);
        } m_spline.smoothHandles(); m_spline.update();
        m_spline2.smoothHandles(); m_spline2.update();
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DynamicRope::draw(RenderWindow& w) { w.draw(m_spline2); w.draw(m_spline); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DynamicRope::destroy() {
        for (size_t i = 1; i < m_segments; i++)
        { PhysicWorld->DestroyBody(m_lstB[i]); }
        delete m_lstB;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//