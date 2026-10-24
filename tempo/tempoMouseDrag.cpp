switch (event.mouseButton.button) {
    case Mouse::Left:
        m_camDragOrigin = m_camera.getCenter();
        float zoom = m_camera.getSize().x / WIN_WIDTH;
        m_camDragAnchor = (m_mousePos) - m_camDragOrigin;
        m_camDragAnchor /= zoom;
        m_camDragActive = true; break;
        
        
if (m_camDragActive) {
    Vector2f camPos = m_camera.getCenter();
    float zoom = m_camera.getSize().x / WIN_WIDTH;
    auto mousePos = (Vector2f)Mouse::getPosition(m_window);
    mousePos -= WIN_CENTER; //mousePos *= zoom;
    Vector2f pos = camPos + mousePos - m_camDragAnchor;
    m_camDragVel.x = ((0 - pos.x) + m_camDragOrigin.x) * 200;
    m_camDragVel.y = ((0 - pos.y) + m_camDragOrigin.y) * 200;
    circTest2.setPosition(camPos);
    camPos += m_camDragVel * dt;
    m_camera.setCenter(camPos);
} else if (abs(m_camDragVel.x) > 0.1f || abs(m_camDragVel.x) > 0.1f) {
    Vector2f camPos = m_camera.getCenter();
    camPos += m_camDragVel * dt;
    DampRef(m_camDragVel, 10.f, dt);
    m_camera.setCenter(camPos);
}