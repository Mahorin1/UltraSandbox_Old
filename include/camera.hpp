//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #pragma once
    #include <utils.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    using CamMoved_CB = std::function<void()>;
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class Camera {
    //-------------//
    public:
        Camera();
        void init(sf::RenderWindow* w, sf::Vector2f dims, sf::Vector2f pos = ZeroVec2f);
        void shake(float f = 10.f, float d = 1.f);
        sf::Vector2f toCamera(sf::Vector2i p);
        sf::Vector2i toScreen(sf::Vector2f p);
        void set(); void unset();
        void update(float dt);
        sf::Vector2f getMouse();
    //-------------//
        void setZoom(float z);
        const float getZoom();
        void setPos(sf::Vector2f p);
        const sf::Vector2f getPos();
        void setAngle(float a);
        float getAngle();
    //-------------//
        sf::Vector2f getTarget();
        void target(sf::Vector2f p);
        void enableTarget(bool b = true);
        void setDragMode(bool b);
        bool getDragMode(); void updateDrag(sf::Event::MouseMoveEvent e);
        CamMoved_CB MovedCB;
    //-------------//
    private:
        float m_zoom;
        float m_angle;
        sf::Vector2f m_dims;
        sf::Vector2f m_pos;
        sf::View m_defaultView;
        sf::View m_cameraView;
        sf::RenderWindow* m_window;
    //-------------//
        sf::Vector2f m_shakePos;
        float m_shakeDamping;
        float m_shakeForce;
        bool m_shakeActive;
    //-------------//
        bool m_targetMode;
        bool m_dragMode;
        b2Body* m_body;
        b2MouseJoint* m_joint;
        sf::Vector2f m_lastMouse;
        sf::Vector2f m_target;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//