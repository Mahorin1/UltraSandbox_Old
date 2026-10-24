//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #pragma once
    #include <physic.hpp>
    #include <sw/Line.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    enum class Ragdoll_M : size_t {
        Head = 0,
        Chest = 1,
        BackArm1 = 2,
        BackArm2 = 3,
        FrontArm1 = 4,
        FrontArm2 = 5,
        BackLeg1 = 6,
        BackLeg2 = 7,
        FrontLeg1 = 8,
        FrontLeg2 = 9,
        Foot1 = 10,
        Foot2 = 11
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
    struct IK_Anim {
    //-------------//
        bool m_active = false;
        sf::Vector2f m_point = ZeroVec2f;
        sf::Vector2f m_dims;
    //-------------//
        sf::Vector2f compute(sf::Vector2f pos, float dir) {
            sf::Vector2f angles;
            auto midPoint = IntersectTwoCircles(pos, m_point, m_dims.x, m_dims.y, (dir==-1));
            angles.x = (Math_Angle(pos, midPoint) - RAD90);
            angles.y = (Math_Angle(midPoint, m_point) - RAD90 - angles.x);
            return angles;
        }
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class Articulation {
    //-------------//
    public:
        Articulation(Collider* c1, Collider* c2, sf::Vector2f& pos, float angle_offset = 0.f);
    //-------------//
        b2RevoluteJoint* getJoint();
        float getTarget();
        float getAngle();
    //-------------//
        void setParams(float speed, float force);
        void setTarget(float a);
        void setActive(bool b);
        void setShortMode(bool b);
    //-------------//
        void destroy(Articulation* &ptr);
        void update();
    //-------------//
    private:
        bool m_shortMode;
        bool m_active;
        float m_speed;
        float m_target;
        b2RevoluteJoint* m_joint;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class Ragdoll {
    //-------------//
    public:
        Ragdoll(sf::Vector2f pos, float size = 1.f);
    //-------------//
        void Remove_A(size_t i);
        void Remove_M(size_t i);
    //-------------//
        void Walk(float gameSpeed);
        void Idle();
    //-------------//
        void destroy(Ragdoll* &ptr);
        void update(float dt, sf::Vector2f mousePos, float gameSpeed = 1.f);
    //-------------//
        void draw(sf::RenderWindow& w);
        ShapedCollider* getM(size_t i);
        void setDir(int8_t dir);
        void setActive(bool b);
    //-------------//
    private:
    //-------------//
        sf::CircleShape m_debugPoint1;
        sf::CircleShape m_debugPoint2;
        sw::Line m_debugLine;
    //-------------//
        IK_Anim m_Arm1_IK;
        IK_Anim m_Leg1_IK;
        IK_Anim m_Arm2_IK;
        IK_Anim m_Leg2_IK;
        float m_chestMidY;
        float m_legDimY;
        float m_walkPointAngle;
        float m_groundDist;
        UserRayCast_CB m_rayCB;
        bool m_onGround;
        int m_nbContact;
        Collider* m_footSensor1;
        Collider* m_footSensor2;
        b2MouseJoint* m_sensorJ1;
        b2MouseJoint* m_sensorJ2;
        float m_wakeUpTimer;
        bool m_wakeUp;
        float m_footStep;
    //-------------//
        bool m_active;
        float m_animTime;
        PIDcontroller m_balancer;
        PIDcontroller m_maintainer;
        float m_angle = 0; int8_t m_dir;
        sf::Clock m_walkTimer; int m_walkPhase;
        std::array<Articulation*, 11> m_lstA;
        std::array<ShapedCollider*, 12> m_lstM;
    //-------------//
        void Rest() {
            setActive(false);
            m_wakeUp = true;
            m_wakeUpTimer = 0.f;
        }
    //-------------//
        bool checkM(size_t i) { return m_lstM[i]!=nullptr; }
        bool checkA(size_t i) { return m_lstA[i]!=nullptr; }
    //-------------//
        b2RevoluteJoint* getJ(size_t i)
        { return checkA(i) ? m_lstA[i]->getJoint() : nullptr; }
    //-------------//
        b2Body* getB(size_t i)
        { return checkM(i) ? m_lstM[i]->getBody() : nullptr; }
    //-------------//
        void setJ(size_t i, float a, float s = 10.f, float f = 5.f)
        { if (checkA(i)) { m_lstA[i]->setTarget(a); m_lstA[i]->setParams(s, f); } }
    //-------------//
        void addJ(size_t i, float a)
        { if (checkA(i)) { m_lstA[i]->setTarget(m_lstA[i]->getTarget() + a); } }
    //-------------//
        void drawM(size_t i, sf::RenderWindow& w)
        { if (checkM(i)) { m_lstM[i]->draw(w); } }
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
