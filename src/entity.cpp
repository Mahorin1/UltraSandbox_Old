//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #include <entity.hpp>
    #include <memory>
//~~~~~~~~~~~~~~~~~~~~~~~~//
    using namespace sf;
    using namespace sw;
    using namespace std;
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    SpawnEntity::SpawnEntity(Collider* c, Vector2f origin)
    : m_bufferPtr(nullptr), m_angleOffset(0.f), m_soundSpeed(1.f), m_supr(false), m_animSupr(false), m_corps(c) {
        m_color = Color::White;
        m_corps->setUserData((uintptr_t)this);
        m_transform.setOrigin(origin);
        m_transform.setPosition(c->getPos());
        m_transform.setScale(0.f, 0.f);
        m_corps->Begin_CB = [this](Collider* collider, b2Contact* contact) -> void {
            if (m_soundClock.getElapsedTime().asSeconds() > 0.2f && !(m_bufferPtr == nullptr)) {
                float v = Clip(Map_Range(m_corps->getTotalVel(), 0.f, 4000.f, 0.f, 100.f), 0.f, 100.f);
                float p = (1-Map_Range(m_corps->getDensity(), 1.f, 50.f, -0.38f, 0.38f)) + Rdm(-0.2f, 0.2f);
                p -= ((1.f - m_soundSpeed)/1.8f); VecSound_Play(m_bufferPtr, v, p); m_soundClock.restart();
            }
        };
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void SpawnEntity::setSoundBuffer(SoundBuffer* sb) { m_bufferPtr = sb; }
    void SpawnEntity::setSoundSpeed(float s) { m_soundSpeed = s; }
    Collider* SpawnEntity::getCorps() { return m_corps; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    bool SpawnEntity::is_supr(bool anim)
    { return (anim) ? m_animSupr : m_supr; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void SpawnEntity::supr(bool anim) {
        if (anim) {
            m_animSupr = true;
            float s = m_transform.getScale().x;
            m_corps->getBody()->SetEnabled(false);
            m_tween.set(tweeny::from(s, 255.f).to(s * 1.3f, 0.f).during(500).via(tweeny::easing::quinticOut));
            m_every->add([this]() -> bool {
                auto v = m_tween.update<array<float, 2>>();
                m_transform.setScale(v[0], v[0]);
                auto col = (uint8_t)v[1];
                m_color = Color(255, col, col, col);
                if (!m_tween.isActive())
                { m_supr = true; return true; }
                return false;
            });
        } else { m_supr = true; if (!m_tween.isActive()) { destroy(); } }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void SpawnEntity::destroy() {
        m_corps->destroy(m_corps);
        m_bufferPtr = nullptr;
        delete this;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f SpawnEntity::getPos() { return m_pos; }
	float SpawnEntity::getAngle() { return m_angle; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void SpawnEntity::update(float dt) {
        m_pos = m_corps->getPos();
        m_angle = m_corps->getAngle();
        m_transform.setPosition(m_pos);
        m_transform.setRotation(m_angle + m_angleOffset);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void SpawnEntity::setEvery(Every* e) {
        m_every = e;
        e->add([this]() -> bool {
            if (m_animSupr) { return true; }
            if (m_supr) { destroy(); return true; }
            auto v = m_tween.update<array<float, 2>>();
            m_transform.setScale(v[0], v[0]); m_angleOffset = v[1];
            return !m_tween.isActive();
        });
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    PhysicRect::PhysicRect(Vector2f pos, Vector2f dims)
    : SpawnEntity(NewRectCollider(dims, pos), dims) {
    //===========//
        b2Fixture* f = m_corps->getFixture();
        f->SetDensity(Rdm(1.f, 50.f));
        f->SetFriction(6.f);
        f->SetRestitution(0.65f);
        b2Body* b = m_corps->getBody();
        b->SetAngularDamping(1.5f);
    //===========//
        RectangleShape Shape;
        Shape.setSize(dims);
        Shape.setOutlineColor(Color::Black);
        Shape.setOutlineThickness(3.f);
        Shape.setOrigin(dims/2.f);
        Shape.setPosition(dims);
    //===========//
        VertexArray Gradient;
        Gradient.setPrimitiveType(Quads); Gradient.resize(4);
        Gradient[0].position = Vector2f(dims.x/2, dims.y/2);
        Gradient[1].position = Vector2f(dims.x + dims.x/2, dims.y/2);
        Gradient[2].position = Vector2f(dims.x + dims.x/2, dims.y + dims.y/2);
        Gradient[3].position = Vector2f(dims.x/2, dims.y + dims.y/2);
        Gradient[0].color = HSVtoRGB(Rdm(0.f, 360.f), 1.f, 1.f);
        Gradient[1].color = HSVtoRGB(Rdm(0.f, 360.f), 1.f, 1.f);
        Gradient[2].color = HSVtoRGB(Rdm(0.f, 360.f), 1.f, 1.f);
        Gradient[3].color = HSVtoRGB(Rdm(0.f, 360.f), 1.f, 1.f);
    //===========//
        m_texture.create(dims.x*2, dims.y*2);
        m_texture.clear(Color::Transparent);
            m_texture.draw(Shape);
            m_texture.draw(Gradient);
        m_texture.display();
        m_sprite.setTexture(m_texture.getTexture());
    //===========//
        m_angleOffset = 0.f;
        m_tween.set(tweeny::from(0.f, 0.f).to(1.f, 0.f).during(1500).via(tweeny::easing::elasticOut));
    //===========//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void PhysicRect::draw(RenderWindow& w) {
        m_sprite.setColor(m_color);
        w.draw(m_sprite, m_transform.getTransform());
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Barrel::Barrel(Vector2f pos, Texture* t, float size)
    : SpawnEntity(NewRectCollider((Vector2f)t->getSize() * size, pos), (Vector2f)t->getSize()/2.f) {
    //===========//
        b2Fixture* f = m_corps->getFixture();
        f->SetDensity(Rdm(1.f, 50.f));
        f->SetFriction(6.f);
        f->SetRestitution(0.65f);
        b2Body* b = m_corps->getBody();
        b->SetAngularDamping(1.5f);
    //===========//
        m_shape.setSize((Vector2f)t->getSize());
        m_shape.setOutlineColor(Color::Black);
        m_shape.setOutlineThickness(3.f);
        m_shape.setTexture(t);
    //===========//
        m_angleOffset = 0.f;
        m_tween.set(tweeny::from(0.f, 0.f).to(size, 0.f).during(1500).via(tweeny::easing::elasticOut));
    //===========//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Barrel::draw(RenderWindow& w) {
        if (m_animSupr) {
            m_shape.setFillColor(m_color);
            m_shape.setOutlineColor(Color(0, 0, 0, m_color.a));
        }
        w.draw(m_shape, m_transform.getTransform());
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Saw::Saw(Vector2f pos, Texture* t, float size)
    : SpawnEntity(NewCircCollider((float)t->getSize().x * size / 2.f, pos), (Vector2f)t->getSize()/2.f) {
    //===========//
        b2Fixture* f = m_corps->getFixture();
        f->SetDensity(Rdm(1.f, 50.f));
        f->SetFriction(100.f);
        f->SetRestitution(0.65f);
        b2Body* b = m_corps->getBody();
        b->SetGravityScale(1.5f);
    //===========//
        m_sprite.setTexture(*t);
    //===========//
        m_angleOffset = 0.f;
        m_tween.set(tweeny::from(0.f, 0.f).to(size, 0.f).during(1500).via(tweeny::easing::elasticOut));
    //===========//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Saw::draw(RenderWindow& w) {
        m_sprite.setColor(Color(0, 0, 0, m_color.a));
        Vector2f s = m_transform.getScale();
        m_transform.setScale(s * 1.1f);
        w.draw(m_sprite, m_transform.getTransform());
        m_sprite.setColor(m_color);
        m_transform.setScale(s);
        w.draw(m_sprite, m_transform.getTransform());
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Robot::Robot(Vector2f pos): m_speed(0.f), m_onGround(true), m_nbContact(0) {
    //-------------//
        m_chest = NewBox(26.f, (pos - Vector2f(0.f, 28.f)));
        m_wheel = NewCirc(18.f, pos);
    //-------------//
        auto sh1 = m_chest->getShape();
        auto sh2 = m_wheel->getShape();
        //sh1->setFillColor(Color::Transparent);
        //sh2->setFillColor(Color::Transparent);
        sh1->setOutlineColor(Color::Black);
        sh2->setOutlineColor(Color::Black);
        sh1->setOutlineThickness(2.f);
        sh2->setOutlineThickness(2.f);
    //-------------//
        m_circ = CircleShape(3.f);
        m_circ.setOrigin(3.f, 3.f);
        m_circ.setFillColor(Color::Black);
    //-------------//
        auto b1 = m_chest->getBody();
        auto b2 = m_wheel->getBody();
        m_joint = NewWheelJoint(b1, b2, pos, b2Vec2(0.f, 1.f));
    //-------------//
        m_balancer.setPID(0.5f, 0.f, 0.2f);
        m_balancer.setOutputLimit(-1.5f, 1.5f);
    //-------------//
		float dampingRatio = 0.4f;
		float omega = PI2 * 4.0f;
        float stiffness = b2->GetMass() * omega * omega;
        float damping = 2.f * b2->GetMass() * dampingRatio * omega;
        m_joint->SetStiffness(stiffness);
        m_joint->SetDamping(damping);
    //-------------//
        m_joint->EnableLimit(true);
        m_joint->EnableMotor(true);
        m_joint->SetMotorSpeed(0.f);
        m_joint->SetMaxMotorTorque(100.f);
        auto limits = sf_to_b2(Vector2f(-18.f, 6.5f));
        m_joint->SetLimits(limits.x, limits.y);
        m_wheel->getFixture()->SetFriction(7.f);
        m_chest->getFixture()->SetFriction(1000.f);
        m_keySwitch = false, m_jumpSwitch = false;
        m_accel = 30.f; m_maxSpeed = 50.f;
    //-------------//
        m_wheel->Begin_CB = [this](Collider* cl, b2Contact* cn) -> void {
            m_nbContact++; m_onGround = true;
        };
        m_wheel->End_CB = [this, stiffness, damping](Collider* cl, b2Contact* cn) -> void {
            m_nbContact--; if (m_nbContact <= 0) {
                m_nbContact = 0; m_onGround = false;
                if (m_jumpSwitch) {
                    m_jumpSwitch = false;
                    m_joint->SetStiffness(stiffness);
                    m_joint->SetDamping(damping);
                }
            }
        };
    //-------------//
        m_posKeep = 0.f;
        m_posKeepMode = false;
        m_posKeeper.setPID(0.6f, 0.f, 0.4f);
        m_posKeeper.setOutputLimit(-10.f, 10.f);
        m_chest->getFixture()->SetDensity(0.1f);
        b2->ResetMassData();
        b1->SetGravityScale(1.f);
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
	void Robot::update(float dt) {
    //-------------//
        m_chest->update(); m_wheel->update();
        auto p = m_wheel->getPos();
        p.x += cosf(m_wheel->getAngle(false)) * 18.8f;
        p.y += sinf(m_wheel->getAngle(false)) * 18.8f;
        m_circ.setPosition(p);
        auto b1 = m_chest->getBody();
        auto b2 = m_wheel->getBody();
    //-------------//
        auto a = m_chest->getAngle(false);
        float input = m_balancer.update(a, 0.f, dt, true);
        b1->ApplyTorque(input * 5.f, true);
        //b1->ApplyTorque(input * 6000.f, true);
    //-------------//
        if (!m_posKeepMode) {
            if (Keyboard::isKeyPressed(Keyboard::A)) {
                if (!m_onGround)
                { b2->ApplyForceToCenter(b2Vec2(-2.5f, 0.f), true); }
                m_speed -= m_accel * dt;
                if (m_speed > 0.f) { m_speed = 0.f; }
                else if (m_speed < -m_maxSpeed)
                { m_speed = -m_maxSpeed; }
            } else if (Keyboard::isKeyPressed(Keyboard::D)) {
                if (!m_onGround)
                { b2->ApplyForceToCenter(b2Vec2(2.5f, 0.f), true); }
                m_speed += m_accel * dt;
                if (m_speed < 0.f) { m_speed = 0.f; }
                else if (m_speed > m_maxSpeed)
                { m_speed = m_maxSpeed; }
            } else { DampRef(m_speed, 3.f, dt); }
            if (Keyboard::isKeyPressed(Keyboard::Space) && !m_keySwitch) {
                float omega = PI2 * 1.1f; m_keySwitch = true;
                m_joint->SetStiffness(b2->GetMass() * omega * omega);
                m_joint->SetDamping(2.9f * b2->GetMass() * omega);
            } else if (!Keyboard::isKeyPressed(Keyboard::Space) && m_keySwitch) {
                float omega = PI2 * 60.f; m_keySwitch = false;
                m_joint->SetDamping(0.f); m_jumpSwitch = true;
                m_joint->SetStiffness(b2->GetMass() * omega * omega);
            } if (Keyboard::isKeyPressed(Keyboard::P) && !m_keySwitch2)
            { m_posKeepMode = true; m_posKeep = m_wheel->getPos().x; }
        } else {
        //-------------//
            auto x = m_wheel->getPos().x;
            m_speed = m_posKeeper.update(x, m_posKeep, dt) * 3.f;
            if (!m_onGround)
            { b2->ApplyForceToCenter(b2Vec2(m_speed/10.f, 0.f), true); }
            /*auto y = m_chest->getPos().y;
            float input = m_posKeeper.update(y, 0.f, dt);
            b1->ApplyForceToCenter(b2Vec2(0.f, input * 1000.f), true);*/
        //-------------//
            if (Keyboard::isKeyPressed(Keyboard::P) && !m_keySwitch2)
            { m_posKeepMode = false; m_posKeep = 0.f; }
        //-------------//
        } m_joint->SetMotorSpeed(m_speed);
        m_keySwitch2 = Keyboard::isKeyPressed(Keyboard::P);
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
	void Robot::draw(RenderWindow& w)
    { m_wheel->draw(w); w.draw(m_circ); m_chest->draw(w); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Guirlande::Guirlande() {
    //-------------//
        const auto r = Rdm(1.5f, 4.f);
        m_circle = CircleShape(r);
        m_circle.setOrigin(r, r);
        m_circle.setOutlineColor(Color::Black);
        m_circle.setFillColor(HSVtoRGB(Rdm(0.1f, 358.f), 1.f, 1.f));
        m_circle.setOutlineThickness(2.f);
    //-------------//
        m_line = Line();
        m_line.setColor(Color::Black);
        m_line.setThickness(2.f);
    //-------------//
        m_bodyA = NewBody(ZeroVec2f, 100.f, 0.f);
        m_bodyB = NewBody(ZeroVec2f, 0.1f, 0.f);
        m_bodyA->SetGravityScale(0.f);
        m_bodyB->SetLinearDamping(1.f);
        m_bodyB->SetGravityScale(2.f);
    //-------------//
        m_linkJ = NewLink(m_bodyA, m_bodyB);
        m_linkJ->SetMinLength(0.f);
        m_linkJ->SetMaxLength(sf_to_b2(Rdm(5.f, 55.f)));
        m_anchorJ = NewMouseJoint(m_bodyA, ZeroVec2f, 100.f, 0.1f, 1000000.f);
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Guirlande::update(Vector2f p) {
    //-------------//
        m_anchorJ->SetTarget(sf_to_b2(p));
        auto p2 = b2_to_sf(m_bodyB->GetPosition());
        m_line.setPoints(p, p2);
        m_circle.setPosition(p2);
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Guirlande::draw(RenderWindow& w)
    { w.draw(m_line); w.draw(m_circle); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//