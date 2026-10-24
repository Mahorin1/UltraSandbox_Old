//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #pragma once
    #include <utils.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class Particule {
    public:
    //-------------//
        Particule(sf::Vector2f pos, sf::Texture& t, sf::Vector2f vel = sf::Vector2f(0.f, 0.f), float life = 1.f, float size = 1.f);
    //-------------//
        void update(float dt);
        void setColor(sf::Color col);
        void setGravity(float g);
        void setDamping(float d, bool angle = false);
        void setAngleVel(float v);
        float getAngleVel();
        void setVel(sf::Vector2f v);
        void setAngle(float a);
        sf::Vector2f getVel();
        void supr();
        bool isSupr();
        void draw(sf::RenderWindow& w);
    //-------------//
    private:
        bool m_isSupr;
        float m_gravity;
        float m_size;
        float m_life;
        float m_angle;
        float m_angleVel;
        float m_angleDamping;
        float m_damping;
        sf::Clock m_timer;
        sf::Vector2f m_pos;
        sf::Vector2f m_vel;
        sf::Sprite m_sprite;
        std::function<bool(float)> m_suprLambda;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    /* TODO
        - ParticleContainer Class
        - ParticleEmitter Class
    */
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//