//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #include <ragdoll.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
    using namespace sf;
    using namespace std;
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Articulation::Articulation(Collider* c1, Collider* c2, Vector2f& pos, float angle_offset)
    : m_active(false), m_target(0.f), m_speed(10.f), m_shortMode(false) {
        m_joint = NewBearing(c1->getBody(), c2->getBody(), pos, toRad(angle_offset));
        m_joint->SetMaxMotorTorque(10.f);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2RevoluteJoint* Articulation::getJoint() { return m_joint; }
    float Articulation::getAngle() { return toDeg(m_joint->GetJointAngle()); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Articulation::setTarget(float a) { m_target = toRad(a); }
    void Articulation::setShortMode(bool b) { m_shortMode = b; }
    void Articulation::setActive(bool b) { m_active = b; m_joint->EnableMotor(b); }
    void Articulation::setParams(float speed, float force)
    { m_speed = speed; m_joint->SetMaxMotorTorque(force); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Articulation::destroy(Articulation* &ptr) {
        PhysicWorld->DestroyJoint(m_joint);
        m_joint = nullptr; ptr = nullptr;
        delete this;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Articulation::update() {
        if (m_active) {
            float a = m_joint->GetJointAngle();
            float dif = (m_shortMode) ? (ShortestAngleDif(a, m_target)) : (a - m_target);
            m_joint->SetMotorSpeed(-m_speed * dif);
        }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Ragdoll::Ragdoll(Vector2f pos, float size) {
    //-------------//
        float Head_Radius = 13.25f * size;
        Vector2f Chest_Dims(11.5f * size, 43.5f * size);
        Vector2f BackArm_Dims(6.f * size, 33.f * size);
        Vector2f FrontArm_Dims(4.8f * size, 32.f * size);
        Vector2f BackLeg_Dims(8.85f * size, 39.f * size);
        Vector2f FrontLeg_Dims(6.75f * size, 35.5f * size);
        Vector2f Foot_Dims(15.f * size, 4.33f * size);
    //-------------//
        m_Arm1_IK.m_dims.x = BackArm_Dims.y;
        m_Arm1_IK.m_dims.y = FrontArm_Dims.y;
        m_Arm2_IK.m_dims.x = BackArm_Dims.y;
        m_Arm2_IK.m_dims.y = FrontArm_Dims.y;
    //-------------//
        m_Leg1_IK.m_dims.x = BackLeg_Dims.y;
        m_Leg1_IK.m_dims.y = FrontLeg_Dims.y;
        m_Leg2_IK.m_dims.x = BackLeg_Dims.y;
        m_Leg2_IK.m_dims.y = FrontLeg_Dims.y;
    //-------------//
        m_chestMidY = Chest_Dims.y * 0.5f;
        m_legDimY = FrontLeg_Dims.y + BackLeg_Dims.y + Foot_Dims.y;
    //-------------//
        float arm_offset = (Chest_Dims.y - BackArm_Dims.y)/2 - (2 * size);
        float leg_offset = (Chest_Dims.y - BackLeg_Dims.y)/2 + BackLeg_Dims.y;
        Vector2f Head_Pos(pos.x, pos.y - Chest_Dims.y/2 - Head_Radius - (6 * size));
        Vector2f BackArm_Pos(pos.x, pos.y - arm_offset);
        Vector2f FrontArm_Pos(pos.x, pos.y - arm_offset + BackArm_Dims.y);
        Vector2f BackLeg_Pos(pos.x, pos.y + leg_offset);
        Vector2f FrontLeg_Pos(pos.x, pos.y + leg_offset + (BackLeg_Dims.y - FrontLeg_Dims.y)/2 + FrontLeg_Dims.y);
        Vector2f Foot_Pos(pos.x + Foot_Dims.x/2 - FrontLeg_Dims.x/2 - size, FrontLeg_Pos.y + FrontLeg_Dims.y/2 + Foot_Dims.y/2 + (2.5f * size));
    //-------------//
        Vector2f head_jpos(pos.x, pos.y - Chest_Dims.y/2 - 2);
        Vector2f backarm_jpos(pos.x, pos.y - Chest_Dims.y/2 + (2.f * size));
        Vector2f frontarm_jpos(pos.x, FrontArm_Pos.y - FrontArm_Dims.y/2);
        Vector2f backleg_jpos(pos.x, BackLeg_Pos.y - BackLeg_Dims.y/2);
        Vector2f frontleg_jpos(pos.x, FrontLeg_Pos.y - FrontLeg_Dims.y/2);
        Vector2f foot_jpos(pos.x, Foot_Pos.y);
    //-------------//
        m_lstM[0] = NewCirc(Head_Radius, Head_Pos);
        m_lstM[1] = NewRect(Chest_Dims, pos);
        m_lstM[2] = NewRect(BackArm_Dims, BackArm_Pos);
        m_lstM[3] = NewRect(BackArm_Dims, BackArm_Pos);
        m_lstM[4] = NewRect(FrontArm_Dims, FrontArm_Pos);
        m_lstM[5] = NewRect(FrontArm_Dims, FrontArm_Pos);
        m_lstM[6] = NewRect(BackLeg_Dims, BackLeg_Pos);
        m_lstM[7] = NewRect(BackLeg_Dims, BackLeg_Pos);
        m_lstM[8] = NewRect(FrontLeg_Dims, FrontLeg_Pos);
        m_lstM[9] = NewRect(FrontLeg_Dims, FrontLeg_Pos);
        m_lstM[10] = NewRect(Foot_Dims, Foot_Pos);
        m_lstM[11] = NewRect(Foot_Dims, Foot_Pos);
    //-------------//
        m_lstA[0] = new Articulation(m_lstM[1], m_lstM[0], head_jpos);
        m_lstA[1] = new Articulation(m_lstM[1], m_lstM[2], backarm_jpos);
        m_lstA[2] = new Articulation(m_lstM[1], m_lstM[3], backarm_jpos);
        m_lstA[3] = new Articulation(m_lstM[2], m_lstM[4], frontarm_jpos);
        m_lstA[4] = new Articulation(m_lstM[3], m_lstM[5], frontarm_jpos);
        m_lstA[5] = new Articulation(m_lstM[1], m_lstM[6], backleg_jpos);
        m_lstA[6] = new Articulation(m_lstM[1], m_lstM[7], backleg_jpos);
        m_lstA[7] = new Articulation(m_lstM[6], m_lstM[8], frontleg_jpos);
        m_lstA[8] = new Articulation(m_lstM[7], m_lstM[9], frontleg_jpos);
        m_lstA[9] = new Articulation(m_lstM[8], m_lstM[10], foot_jpos, 90.f);
        m_lstA[10] = new Articulation(m_lstM[9], m_lstM[11], foot_jpos, 90.f);
    //-------------//
        getJ(0)->SetLimits(toRad(-70), toRad(70));
        m_walkPhase = 0; m_animTime = 0.f;
        m_dir = -1; setDir(1);
        m_active = false;
        setActive(true);
        m_walkPointAngle = 0.f;
        m_nbContact = 0;
        m_onGround = false;
        m_wakeUp = false;
        m_footStep = 35.f;
    //-------------//
        for (auto& a : m_lstA)
        { a->setShortMode(true); }
        m_lstA[0]->setShortMode(false);
        m_lstA[9]->setShortMode(false);
        m_lstA[10]->setShortMode(false);
    //-------------//
        m_balancer.setPID(0.8f, 0.1f, 0.2f);
        m_balancer.setOutputLimit(-1.f, 1.f);
        m_maintainer.setPID(0.2f, 0.5f, 0.2f);
        m_maintainer.setOutputLimit(0.f, 1.f);
    //-------------//
        b2MassData md1, md2;
        m_lstM[1]->getFixture()->GetMassData(&md1);
        md2 = m_lstM[1]->getBody()->GetMassData();
        PrintLine();
        PrintT(md1.mass);
        PrintT(md1.I);
        PrintVec(Vector2f(md1.center.x, md1.center.y));
        PrintLine();
        PrintT(md2.mass);
        PrintT(md2.I);
        PrintVec(Vector2f(md2.center.x, md2.center.y));
        PrintLine();
    //-------------//
        m_Leg1_IK.m_active = true;
        m_Leg2_IK.m_active = true;
        m_Leg1_IK.m_point = getM(10)->getPos();
        m_Leg2_IK.m_point = getM(11)->getPos();
    //-------------//
        float radius = 20.f;
        m_debugPoint1.setRadius(radius);
        m_debugPoint1.setOrigin(radius, radius);
        m_debugPoint1.setFillColor(Color::Red);
        m_debugPoint2.setRadius(radius);
        m_debugPoint2.setOrigin(radius, radius);
        m_debugPoint2.setFillColor(Color::Blue);
        m_debugLine.setThickness(1.3f);
        m_debugLine.setColor(Color(255U, 255U, 255U, 200U));
    //-------------//
        for (const auto& m : m_lstM) {
        //-------------//
            m->getFixture()->SetFriction(5.f);
            m->getFixture()->SetRestitution(0.9f);
            m->getFixture()->SetRestitution(0.f);
            m->getShape()->setOutlineThickness(2.f * size);
            m->getShape()->setFillColor(RdmColor());
            m->getShape()->setOutlineColor(Color::Black);
            m->setUserData((uintptr_t)this);
        //-------------//
        }
    //-------------//
        m_lstM[10]->getFixture()->SetFriction(20.f);
        m_lstM[11]->getFixture()->SetFriction(20.f);
        m_lstM[10]->getFixture()->SetRestitution(0.f);
        m_lstM[11]->getFixture()->SetRestitution(0.f);
    //-------------//
        auto end_cb = [this](Collider* cl, b2Contact* cn) -> void {
            m_nbContact--; if (m_nbContact <= 0)
            { m_nbContact = 0; m_onGround = false; }
        }; auto begin_cb = [this](Collider* cl, b2Contact* cn)
        -> void { m_nbContact++; m_onGround = true; };
    //-------------//
        auto sh = NewCircShape(30.f);
        m_footSensor1 = new Collider(sh);
        m_footSensor2 = new Collider(sh);
        m_footSensor1->setSensor();
        m_footSensor2->setSensor();
        m_footSensor1->setUserData((uintptr_t)this);
        m_footSensor2->setUserData((uintptr_t)this);
        m_footSensor1->setGravityScale(0.f);
        m_footSensor2->setGravityScale(0.f);
        m_footSensor1->Begin_CB = begin_cb;
        m_footSensor2->Begin_CB = begin_cb;
        m_footSensor1->End_CB = end_cb;
        m_footSensor2->End_CB = end_cb;
        m_sensorJ1 = NewMouseJoint(m_footSensor1->getBody(), ZeroVec2f, 100.f, 0.1f, 1000000.f);
        m_sensorJ2 = NewMouseJoint(m_footSensor2->getBody(), ZeroVec2f, 100.f, 0.1f, 1000000.f);
    //-------------//
        m_rayCB = [this](Collider* c, RayHit& h) -> float {
            if (c->getUserData()==(uintptr_t)this) { return -1; }
            m_groundDist = abs(h.m_start.y - h.m_point.y);
            return h.m_fraction;
        };
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Ragdoll::setActive(bool b) {
    //-------------//
        if (b && !m_active) {
        //-------------//
            for (auto& a : m_lstA) {
                a->setActive(true);
                a->getJoint()->EnableLimit(true);
            } getJ(1)->EnableLimit(false);
            getJ(2)->EnableLimit(false);
        //-------------//
        } else if (!b && m_active) {
        //-------------//
            for (auto& a : m_lstA) { a->setActive(false); }
            for (auto& m : m_lstM)
            //{ m->setAngle(capRad2(m->getAngle())); }
            getJ(1)->EnableLimit(true);
            getJ(2)->EnableLimit(true);
        //-------------//
        } m_active = b;
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Ragdoll::update(float dt, Vector2f mousePos, float gameSpeed) {
    //-------------//
        m_sensorJ1->SetTarget(getB(10)->GetPosition());
        m_sensorJ2->SetTarget(getB(11)->GetPosition());
    //-------------//
        for (auto& a : m_lstA) { if (a) { a->update(); } }
        for (auto& m : m_lstM) { if (m) { m->update(); } }
    //-------------//
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            setDir(-1); //Walk(gameSpeed);
        } else if (Keyboard::isKeyPressed(Keyboard::D)) {
            setDir(1); //Walk(gameSpeed);
        } else { Idle(); }
    //-------------//
        if (Keyboard::isKeyPressed(Keyboard::Up) && !m_wakeUp) { setActive(true); }
        if (Keyboard::isKeyPressed(Keyboard::Down)) { setActive(false); }
        if (Keyboard::isKeyPressed(Keyboard::I)) { m_footStep+=10*dt; }
        if (Keyboard::isKeyPressed(Keyboard::K)) { m_footStep-=10*dt; }
    //-------------//
        if (m_active) {
        //-------------//
            Vector2f p, angles;
            float s = 10.f, f = 4.f;
            if (m_Arm1_IK.m_active) {
                p = b2_to_sf(getJ(2)->GetAnchorA());
                angles = m_Arm1_IK.compute(p, m_dir);
                setJ(2, toDeg(angles.x), s, f);
                setJ(4, toDeg(angles.y), s, f);
            } if (m_Arm2_IK.m_active) {
                p = b2_to_sf(getJ(1)->GetAnchorA());
                angles = m_Arm2_IK.compute(p, m_dir);
                setJ(1, toDeg(angles.x), s, f);
                setJ(3, toDeg(angles.y), s, f);
            } if (m_Leg1_IK.m_active) {
                p = b2_to_sf(getJ(6)->GetAnchorA());
                angles = m_Leg1_IK.compute(p, -m_dir);
                setJ(6, toDeg(angles.x), s, f);
                setJ(8, toDeg(angles.y), s, f);
            } if (m_Leg2_IK.m_active) {
                p = b2_to_sf(getJ(5)->GetAnchorA());
                angles = m_Leg2_IK.compute(p, -m_dir);
                setJ(5, toDeg(angles.x), s, f);
                setJ(7, toDeg(angles.y), s, f);
            }
        //-------------//
            if (m_onGround) {
            //-------------//
                auto chest_a = getB(1)->GetAngle();
                p = getM(1)->getPos();
                auto a = chest_a + RAD90;
                p.x += cosf(a) * m_chestMidY;
                p.y += sinf(a) * m_chestMidY;
                a = RAD90 - chest_a;
                p.x += cosf(a) * (m_legDimY + 0.f);
                p.y += sinf(a) * (m_legDimY + 0.f);
            //-------------//
                a = m_walkPointAngle;
                auto d = m_footStep;
                auto p2 = p;
                p.x += cosf(a) * d;
                p.y += sinf(a) * d;
                p2.x += cosf(a - b2_pi) * d;
                p2.y += sinf(a - b2_pi) * d;
            //-------------//
                m_Leg1_IK.m_point = p;
                m_Leg2_IK.m_point = p2;
                m_walkPointAngle += 5 * dt * gameSpeed * m_dir;
            //-------------//
                // Self Balance System
                auto power = 5.8f;
                auto input = m_balancer.update(chest_a, 0.f, dt, true);
                getB(1)->ApplyTorque(input * power, true);
                if ((abs(capRad2(chest_a)) > toRad(40.f))) {
                    if (!m_wakeUp) { Rest(); }
                } else if (m_wakeUp) {
                    m_wakeUpTimer += dt;
                    if (m_wakeUpTimer > 4.f)
                    { m_wakeUp = false; }
                }
            //-------------//
                //WorldRayCast(p, (p + Vector2f(0.f, m_legDimY+10.f)), m_rayCB);
                //input = m_maintainer.update(m_groundDist, (m_legDimY+2.f), dt);
                //getB(1)->ApplyForce(b2Vec2(0.f, input*-3.f), sf_to_b2(p), true);
                //getB(1)->ApplyForce(b2Vec2(0.f, -1.f), sf_to_b2(p), true);
                power = m_wakeUp ? 0.5f : 0.2f;
                getB(1)->ApplyForceToCenter(b2Vec2(0.f, -power), true);
            //-------------//
            } else { Rest(); }
        //-------------//
        } else if (m_wakeUp) {
            if (m_onGround) {
                m_wakeUpTimer += dt;
                if (m_wakeUpTimer > 3.f)
                { setActive(true); }
            } else { m_wakeUpTimer = 0.f; }
        }
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Ragdoll::Walk(float gameSpeed) {
    //-------------//
        float s = 8.f, f = 8.f, d = m_dir;
    //-------------//
        if (m_walkTimer.getElapsedTime()>=seconds(m_animTime/gameSpeed)) {
            switch (m_walkPhase) {
            //-------------//
                case 0:
                //=====//

                //=====//
                    setJ(6, -48.f * d, s, f);
                    setJ(8, 95.f * d, s, f);
                    setJ(5, 0.f, s, f);
                    setJ(7, 0.f, s, f);
                    setJ(9, -90.f * d, s, f);
                //=====//
                    m_walkTimer.restart();
                    m_walkPhase = 1;
                    m_animTime = 0.5f;
                    break;
            //-------------//
                case 1:
                //=====//

                //=====//
                    setJ(5, 28.f * d, s, f);
                    setJ(7, 0.f, s, f);
                    setJ(6, -25.f * d, s, f);
                    setJ(8, 0.f, s, f);
                    setJ(10, -90.f * d, s, f);
                //=====//
                    m_walkTimer.restart();
                    m_walkPhase = 2;
                    m_animTime = 0.4f;
                    break;
            //-------------//
                case 2:
                //=====//

                //=====//
                    setJ(5, -45.f * d, s, f);
                    setJ(7, 90.f * d, s, f);
                    setJ(6, 0.f, s, f);
                    setJ(8, 0.f, s, f);
                    setJ(10, -90.f * d, s, f);
                //=====//
                    m_walkTimer.restart();
                    m_walkPhase = 3;
                    m_animTime = 0.5f;
                    break;
            //-------------//
                case 3:
                //=====//

                //=====//
                    setJ(6, 28.f * d, s, f);
                    setJ(8, 0.f, s, f);
                    setJ(5, -25.f * d, s, f);
                    setJ(7, 0.f, s, f);
                    setJ(9, -90.f * d, s, f);
                //=====//
                    m_walkTimer.restart();
                    m_walkPhase = 0;
                    m_animTime = 0.4f;
                    break;
            //-------------//
            }
        }
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Ragdoll::Idle() {
    //-------------//
        m_walkPhase = 0; m_animTime = 0.f;
        const float s = 5.f, f = 5.f;
    //-------------//
        setJ(1, 8.f * m_dir, s, f);
        setJ(2, 8.f * m_dir, s, f);
        setJ(3, -28.f * m_dir, s, f);
        setJ(4, -28.f * m_dir, s, f);
    //-------------//
        setJ(0, 0.f, s, f);
        setJ(5, 0.f, s, f);
        setJ(6, 0.f, s, f);
        setJ(7, 0.f, s, f);
        setJ(8, 0.f, s, f);
        setJ(9, -90.f * m_dir, 1.f, 1.f);
        setJ(10, -90.f * m_dir, 1.f, 1.f);
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Ragdoll::setDir(int8_t dir) {
    //-------------//
        if (dir == 1 && m_dir == -1) {
            getJ(1)->SetLimits(toRad(-215), toRad(65));
            getJ(2)->SetLimits(toRad(-215), toRad(65));
            getJ(3)->SetLimits(toRad(-150), toRad(0));
            getJ(4)->SetLimits(toRad(-150), toRad(0));
            getJ(5)->SetLimits(toRad(-140), toRad(60));
            getJ(6)->SetLimits(toRad(-140), toRad(60));
            getJ(7)->SetLimits(toRad(0), toRad(140));
            getJ(8)->SetLimits(toRad(0), toRad(140));
            getJ(9)->SetLimits(toRad(-120), toRad(-50));
            getJ(10)->SetLimits(toRad(-120), toRad(-50));
            getM(10)->setAngle(0.f); getM(11)->setAngle(0.f);
            m_dir = dir; Idle();
        } else if (dir == -1 && m_dir == 1) {
            getJ(1)->SetLimits(toRad(-65), toRad(215));
            getJ(2)->SetLimits(toRad(-65), toRad(215));
            getJ(3)->SetLimits(toRad(0), toRad(150));
            getJ(4)->SetLimits(toRad(0), toRad(150));
            getJ(5)->SetLimits(toRad(-60), toRad(140));
            getJ(6)->SetLimits(toRad(-60), toRad(140));
            getJ(7)->SetLimits(toRad(-140), toRad(0));
            getJ(8)->SetLimits(toRad(-140), toRad(0));
            getJ(9)->SetLimits(toRad(50), toRad(120));
            getJ(10)->SetLimits(toRad(50), toRad(120));
            getM(10)->setAngle(180.f); getM(11)->setAngle(180.f);
            m_dir = dir; Idle();
        }
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Ragdoll::draw(RenderWindow& w) {
    //-------------//
        drawM(2, w); drawM(4, w);// Back Arm
    //-------------//
        drawM(6, w); drawM(8, w); drawM(10, w);// Back Leg
    //-------------//
        drawM(1, w); drawM(0, w);// Cheast / Head
    //-------------//
        drawM(7, w); drawM(9, w); drawM(11, w);// Front Leg
    //-------------//
        drawM(3, w); drawM(5, w);// Front Arm
    //-------------//
        w.draw(m_debugLine);
        w.draw(m_debugPoint1);
        w.draw(m_debugPoint2);
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Ragdoll::Remove_A(size_t i)
    { if (checkA(i)) { m_lstA[i]->destroy(m_lstA[i]); } }
    void Ragdoll::Remove_M(size_t i)
    { if (checkM(i)) { m_lstM[i]->destroy(m_lstM[i]); } }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Ragdoll::destroy(Ragdoll* &ptr) {
        m_footSensor1->destroy(m_footSensor1);
        m_footSensor2->destroy(m_footSensor2);
        for (auto& a : m_lstA) { if (a) { a->destroy(a); } }
        for (auto& m : m_lstM) { if (m) { m->destroy(m); } }
        ptr = nullptr; delete this;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    ShapedCollider* Ragdoll::getM(size_t i) { return m_lstM.at(i); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//===========================================================================//