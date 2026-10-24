//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #include <camera.hpp>
    #include <physic.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
    using namespace sf;
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Camera::Camera(): m_shakePos(ZeroVec2f), m_angle(0.f), m_zoom(1.f),
    m_targetMode(false), m_target(ZeroVec2f), m_shakeActive(false),
    m_dragMode(false), m_pos(ZeroVec2f) {}
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Camera::init(RenderWindow* w, Vector2f dims, Vector2f pos) {
        m_window = w; m_defaultView = w->getDefaultView();
        m_pos = pos; m_dims = dims; m_cameraView.setSize(dims);
        m_body = NewBody(); m_body->SetEnabled(false);
        m_joint = NewMouseJoint(m_body, ZeroVec2f, 250.f, 0.1f, 10000000.f);
        m_body->SetGravityScale(0.f);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Camera::setZoom(float z) {
        if (m_dragMode) {
            auto lm = m_lastMouse; lm -= WIN_CENTER;
            m_target += ((lm * m_zoom) - (lm * z));
        } m_zoom = z; m_cameraView.setSize(m_dims * z);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    const float Camera::getZoom() { return m_zoom; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Camera::setPos(Vector2f pos) { m_pos = pos; }
    const Vector2f Camera::getPos() { return m_pos; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Camera::setAngle(float a) { m_angle = a; }
    float Camera::getAngle() { return m_angle; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Camera::update(float dt) {
    //-------------//
        if (m_body->IsEnabled()) {
            if (m_dragMode) {
                m_pos = m_target;
            } else {
                m_pos = b2_to_sf(m_body->GetPosition());
            }

            if (m_targetMode) {
                m_joint->SetTarget(sf_to_b2(m_target));
            } else {
                //m_body->SetEnabled(false);
            }
        }
        
    //-------------//
        m_cameraView.setRotation(m_angle);
        m_cameraView.setCenter(m_pos + m_shakePos);
        if (m_shakeActive) {
            m_shakePos.x = 0.f;
            m_shakePos.y = 0.f;
            if (m_shakeForce < 0.2f)
            { m_shakeActive = false; } else {
                m_shakePos.x += Rdm(0-m_shakeForce, m_shakeForce);
                m_shakePos.y += Rdm(0-m_shakeForce, m_shakeForce);
                DampRef(m_shakeForce, m_shakeDamping, dt);
            }
        }
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f Camera::toCamera(Vector2i p)
    { return m_window->mapPixelToCoords(p, m_cameraView); }
    Vector2i Camera::toScreen(Vector2f p)
    { return m_window->mapCoordsToPixel(p, m_cameraView); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Camera::enableTarget(bool b) {
        m_targetMode = b;
        m_target = m_pos;
        if (b) {
            m_body->SetEnabled(true);
            m_joint->SetMaxForce(10000.f);
            m_body->SetLinearDamping(0.f);
            m_body->SetTransform(sf_to_b2(m_pos), 0.f);
        } else {
            m_joint->SetMaxForce(0.f);
            m_body->SetLinearDamping(5.f);
        }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f Camera::getTarget() { return m_target; }
    void Camera::target(Vector2f p) { m_target = p; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Camera::set() { m_window->setView(m_cameraView); }
    void Camera::unset() { m_window->setView(m_defaultView); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Camera::shake(float f, float d) {
        m_shakePos.x = 0.f, m_shakePos.y = 0.f;
        m_shakeForce = f; m_shakeDamping = d;
        m_shakeActive = true;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f Camera::getMouse()
    { return toCamera(Mouse::getPosition(*m_window)); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    bool Camera::getDragMode() { return m_dragMode; }
    void Camera::setDragMode(bool b) {
        enableTarget(b);
        m_dragMode = b; if (b) {
            m_lastMouse = (Vector2f)Mouse::getPosition(*m_window);
        } else {
            m_body->SetTransform(sf_to_b2(m_pos), 0.f);
            b2Vec2 vel = m_body->GetLinearVelocity();
            //vel.x /= m_zoom; vel.y /= m_zoom;
            PrintT(m_zoom);
            //m_body->SetLinearVelocity(vel);
        }
    } void Camera::updateDrag(Event::MouseMoveEvent e) {
        if (m_dragMode) {
            Vector2f p(e.x, e.y);
            m_target += (m_lastMouse - p) * m_zoom;
            m_lastMouse = p;
        }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//