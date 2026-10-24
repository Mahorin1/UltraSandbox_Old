//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #include <particle.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
    using namespace sf;
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Particule::Particule(Vector2f pos, Texture& t, Vector2f vel, float life, float size)
    : m_pos(pos), m_vel(vel), m_size(size), m_life(life), m_isSupr(false) {
        m_timer.restart(); m_sprite.setTexture(t);
        m_sprite.setOrigin((Vector2f)t.getSize());
        m_damping = 0.f; m_angleDamping = 0.f;
        m_angle = 0.f; m_angleVel = 0.f;
        m_suprLambda = [this](float dt) -> bool { return true; };
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Particule::update(float dt) {
        m_sprite.setScale(m_size, m_size);
        m_sprite.setRotation(m_angle);
        m_sprite.setPosition(m_pos);
        m_angle += m_angleVel * dt;
        m_pos += m_vel * dt;
        if (m_damping > 0.f) { m_vel = Damp(m_vel, m_damping, dt); }
        if (m_angleDamping > 0.f) { m_angleVel = Damp(m_angleVel, m_angleDamping, dt); }
        if (m_gravity > 0.f) { m_vel.y += m_gravity * 100.f * dt; }
        if (m_timer.getElapsedTime().asSeconds() >= m_life)
        { if (m_suprLambda(dt)) { supr(); } }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Particule::setColor(Color col) { m_sprite.setColor(col); }
    void Particule::setGravity(float g) { m_gravity = g; }
    void Particule::setVel(Vector2f v) { m_vel = v; }
    void Particule::setAngle(float a) { m_angle = a; }
    void Particule::setAngleVel(float v) { m_angleVel = v; }
    void Particule::setDamping(float d, bool angle)
    { if (angle) { m_angleDamping = d; } else { m_damping = d; } }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Particule::getAngleVel() { return m_angleVel; }
    Vector2f Particule::getVel() { return m_vel; }
    bool Particule::isSupr() { return m_isSupr; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Particule::supr() { m_isSupr = true; }
    void Particule::draw(RenderWindow& w) { w.draw(m_sprite); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//