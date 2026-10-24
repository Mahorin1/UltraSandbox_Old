//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #pragma once
    #include <utils.hpp>
    #include <sw/Spline.hpp>
    #include <sw/Line.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2PolygonShape NewRectShape(sf::Vector2f dims);
    b2PolygonShape NewBoxShape(float side);
    b2CircleShape NewCircShape(float radius);
    void InitWorld(float gx, float gy, float step, int32 vel_iter, int32 pos_iter);
    void SetWorldStep(float step, int32 vel_iter, int32 pos_iter);
    void UpdateWorld(float dt, float speed);
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    struct RayHit {
    //-------------//
        RayHit(b2Fixture* f, b2Vec2 p, b2Vec2 n, float fraction, sf::Vector2f sp, sf::Vector2f ep)
        : m_fixture(f), m_point(b2_to_sf(p)), m_normal(n), m_fraction(fraction), m_start(sp), m_end(ep) {}
    //-------------//
        sf::Vector2f m_point;
        b2Vec2 m_normal;
        b2Fixture* m_fixture;
        float m_fraction;
        sf::Vector2f m_start;
        sf::Vector2f m_end;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class Collider; class ShapedCollider;
    extern b2World* PhysicWorld; extern b2Body* JointBody;
    using Collide_CB = std::function<void(Collider*, b2Contact*)>;
    using UserQuery_CB = std::function<bool(sf::FloatRect&, Collider*, b2Fixture*)>;
    using UserRayCast_CB = std::function<float(Collider*, RayHit&)>;
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Collider* NewRectCollider(sf::Vector2f dims, sf::Vector2f pos  = ZeroVec2f, b2BodyType type = b2_dynamicBody);
    Collider* NewBoxCollider(float side, sf::Vector2f pos  = ZeroVec2f, b2BodyType type = b2_dynamicBody);
    Collider* NewCircCollider(float radius, sf::Vector2f pos  = ZeroVec2f, b2BodyType type = b2_dynamicBody);
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    ShapedCollider* NewRect(sf::Vector2f dims, sf::Vector2f pos  = ZeroVec2f, b2BodyType type = b2_dynamicBody);
    ShapedCollider* NewBox(float side, sf::Vector2f pos  = ZeroVec2f, b2BodyType type = b2_dynamicBody);
    ShapedCollider* NewCirc(float radius, sf::Vector2f pos  = ZeroVec2f, b2BodyType type = b2_dynamicBody);
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2RevoluteJoint* NewBearing(b2Body* b1, b2Body* b2, sf::Vector2f pos, float angle_offset = 0.f, bool collide_connected = false);
    b2RevoluteJoint* NewBearing(b2Body* b, sf::Vector2f pos, float angle_offset = 0.f);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2DistanceJoint* NewLink(b2Body* b1, b2Body* b2, sf::Vector2f pos1, sf::Vector2f pos2, float hertz = -1.f, float damp = -1.f, bool collide_connected = false);
    b2DistanceJoint* NewLink(b2Body* b1, b2Body* b2, float hertz = -1.f, float damp = -1.f, bool collide_connected = false);
    b2DistanceJoint* NewLink(b2Body* b, sf::Vector2f pos1, sf::Vector2f pos2, float hertz = -1.f, float damp = -1.f);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2MouseJoint* NewMouseJoint(b2Body* b, sf::Vector2f pos, float hertz = 10.f, float damp = 3.f, float force = 1000.f);
    b2WheelJoint* NewWheelJoint(b2Body* b1, b2Body* b2, sf::Vector2f anchor, b2Vec2 axis, bool collide_connected = false);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2WeldJoint* NewWeldJoint(b2Body* b1, b2Body* b2, sf::Vector2f pos);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2Body* NewBody(sf::Vector2f pos = ZeroVec2f, float mass = 1.f, float inertia = 0.5f);
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void QueryWorldZone(sf::FloatRect& zone, UserQuery_CB user_cb);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2Vec2 GetWorldCentreOfMass(std::vector<b2Body*>& bodyLst);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void WorldRayCast(sf::Vector2f p1, sf::Vector2f p2, UserRayCast_CB user_cb);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    bool SameUserData(Collider*, Collider*);
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class StaticRope {
    //-------------//
    public:
        StaticRope(b2Body* b1, b2Body* b2, sf::Vector2f p1, sf::Vector2f p2, int segments = 30, float length = -1.f);
        sf::Vector2f getAnchor(bool first = true);
        void setLength(float l);
        float getLength();
        sw::Spline* getSpline();
        b2DistanceJoint* getJoint();
        sf::Vector2f getPoint(float t);
        void draw(sf::RenderWindow& w);
        void compute(sf::Vector2f p1, sf::Vector2f p2);
        void destroy(); void update();
    //-------------//
    private:
    //-------------//
        float m_length;
        int m_segments;
        sw::Spline m_spline;
        b2DistanceJoint* m_rope;
        Catenary m_curve;
        int8_t m_lastMode;
        sf::Vector2f m_oldP1;
        sf::Vector2f m_oldP2;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class DynamicRope {
    //-------------//
    public:
        DynamicRope(b2Body* b1, b2Body* b2, sf::Vector2f p1, sf::Vector2f p2, int segments = 10);
        void update(); void destroy(); void draw(sf::RenderWindow& w);
    //-------------//
    private:
        sw::Spline m_spline;
        sw::Spline m_spline2;
        b2Body** m_lstB;
        int m_segments;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class Collider {
    //-------------//
    public:
        Collider(b2Shape& shape, sf::Vector2f pos = ZeroVec2f, b2BodyType type = b2_dynamicBody);
        virtual void destroy(Collider* &ptr);
        bool testPoint(sf::Vector2f pos);
        void setUserData(uintptr_t ptr);
        uintptr_t getUserData();
        float getAngle(bool deg = true);
        void setAngle(float a, bool deg = true);
        sf::Vector2f getPos();
        void setPos(sf::Vector2f pos);
        void setSensor(bool b = true);
        void setGravityScale(float g = 1.f);
        void setType(b2BodyType t);
        bool isSensor();
        float getTotalVel();
        float getDensity();
        b2Body* getBody();
        b2BodyType getType();
        b2Fixture* getFixture();
        Collide_CB Begin_CB;
        Collide_CB End_CB;
        Collide_CB Pre_CB;
        Collide_CB Post_CB;
    //-------------//
    private:
        b2Body* m_body;
        b2Fixture* m_fixture;
        uintptr_t m_userDataPtr;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class ShapedCollider : public Collider {
    //-------------//
    public:
        ShapedCollider(b2Shape& shape, sf::Vector2f pos = ZeroVec2f, b2BodyType type = b2_dynamicBody);
        void destroy(ShapedCollider* &ptr);
        sf::Shape* getShape(); void update();
        void draw(sf::RenderWindow& w);
    //-------------//
    private:
        sf::Vector2f m_pos;
        sf::Shape* m_sfShape;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class DragJoint {
    //-------------//
    public:
        DragJoint(float RayonA = 4.f, float RayonB = 4.f, float LineWidth = 3.5f);
        void setObj(b2Body* body, sf::Vector2f pos);
        sf::Vector2f getAnchor(bool first = true);
        void draw(sf::RenderWindow& w);
        void target(sf::Vector2f pos);
        void setAngleMode(bool b);
        bool sameBody(b2Body* b);
        void rotate(float vel);
        bool getAngleMode();
        b2Body* getBody();
        bool isActive();
        void destroy();
        void update();
        void unSet();
    //-------------//
    private:
        bool m_angleActive;
        float m_saveDamping;
        bool m_saveFixedRot;
        b2MassData m_saveMassData;
        b2MouseJoint* m_joint;
        b2Body* m_targetBody;
    //-------------//
        float m_lineWidth;
        sf::CircleShape m_shapeA;
        sf::CircleShape m_shapeB;
        sf::RectangleShape m_rectLine;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//