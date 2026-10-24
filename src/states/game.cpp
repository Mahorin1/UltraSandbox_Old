//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #include <states/game.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
    using namespace sf;
    using namespace sw;
    using namespace std;
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    SideToolUI::SideToolUI(Every& e, ResourceManager& rm)
    : Panel(Vector2f(WIN_DIMS.x+35.f, WIN_CENTER.y), Vector2f(90.f, 658.f)) {
    //-------------//
        RectangleShape SliderShape(m_dims);
        Color col1(8, 41, 64, 185);
        SliderShape.setFillColor(col1);
        SliderShape.setOutlineColor(Color::Black);
        SliderShape.setOutlineThickness(-2.f);
        m_canvas.clear(Color::Transparent);
            m_canvas.draw(SliderShape);
        m_canvas.display();
    //-------------//
        m_bufferPtr[0] = rm.get<SoundBuffer>("woosh");
        m_bufferPtr[1] = rm.get<SoundBuffer>("Sw_On");
        m_bufferPtr[2] = rm.get<SoundBuffer>("Sw_Off");
    //-------------//
        m_mode = 0;
        m_nbBtn = 10;
        int index = 0;
        auto pos = getPos();
        float by = pos.y-285.f;
        Vector2f btn_dims(60.f, 60.f);
        RectangleShape BtnShape(btn_dims);
        Sprite BtnSprite;
        Color col2(80, 115, 165);
        BtnShape.setFillColor(col2);
        BtnShape.setOutlineColor(Color::Black);
        BtnShape.setOutlineThickness(-2.f);
        for (auto& b : m_btn) {
            b = new Button(Vector2f(pos.x, by), btn_dims, true);
            b->setEvent(GuiEvent::Pressed, [this, index](bool b) -> void {
                float s = m_btn[index]->getSpr().getScale().x;
                if (b) {
                    if (!(m_lastPressed == m_btn[index])) {
                        if (m_lastPressed->isPressed())
                        { m_lastPressed->callPressed(false); }
                    } m_lastPressed = m_btn[index]; m_mode = index+1;
                    m_btnTweens[index].set(tweeny::from(s).to(0.8f).during(400).via(tweeny::easing::elasticOut));
                    VecSound_Play(m_bufferPtr[2], 55.f);
                    VecSound_Play(m_bufferPtr[1], 25.f, 0.7f);
                } else {
                    m_btnTweens[index].set(tweeny::from(s).to(1.f).during(400).via(tweeny::easing::elasticOut));
                    VecSound_Play(m_bufferPtr[2], 55.f); m_mode = 0;
                }
            }); index++; by += 63.5f;
        } m_lastPressed = m_btn[0];
    //-------------//
        for (auto i = 0; i < m_nbBtn; i++) {
        auto b = m_btn[i]; switch (i) {
        //-------------//
            case 0:
                BtnSprite.setTexture(*rm.get<Texture>("Grab"), true);
                BtnSprite.setOrigin(BtnSprite.getLocalBounds().getSize()/2.f);
                BtnSprite.setPosition(b->getDims()/2.f);
                b->getC().setSmooth(true); b->set();
                    b->drawC(BtnShape);
                    b->drawC(BtnSprite);
                b->unset(); break;
        //-------------//
            case 1:
                BtnSprite.setTexture(*rm.get<Texture>("Link"), true);
                BtnSprite.setOrigin(BtnSprite.getLocalBounds().getSize()/2.f);
                BtnSprite.setPosition(b->getDims()/2.f);
                b->getC().setSmooth(true); b->set();
                    b->drawC(BtnShape);
                    b->drawC(BtnSprite);
                b->unset(); break;
        //-------------//
            case 2:
                BtnSprite.setTexture(*rm.get<Texture>("Select"), true);
                BtnSprite.setOrigin(BtnSprite.getLocalBounds().getSize()/2.f);
                BtnSprite.setPosition(b->getDims()/2.f);
                b->getC().setSmooth(true); b->set();
                    b->drawC(BtnShape);
                    b->drawC(BtnSprite);
                b->unset(); break;
        //-------------//
            case 3:
                b->set();
                    b->drawC(BtnShape);
                b->unset(); break;
        //-------------//
            case 4:
                b->set();
                    b->drawC(BtnShape);
                b->unset(); break;
        //-------------//
            case 5:
                b->set();
                    b->drawC(BtnShape);
                b->unset(); break;
        //-------------//
            case 6:
                b->set();
                    b->drawC(BtnShape);
                b->unset(); break;
        //-------------//
            case 7:
                b->set();
                    b->drawC(BtnShape);
                b->unset(); break;
        //-------------//
            case 8:
                b->set();
                    b->drawC(BtnShape);
                b->unset(); break;
        //-------------//
            case 9:
                b->set();
                    b->drawC(BtnShape);
                b->unset(); break;
        //-------------//
        } }
    //-------------//
        m_showBtn = false;
        float x = getPos().x;
        m_posX1 = x, m_posX2 = x-78.f;
    //-------------//
        setEvent(GuiEvent::Hover, [this](bool b) -> void {
            if (b) {
                m_tween.set(tweeny::from(m_posX1).to(m_posX2).during(850).via(tweeny::easing::exponentialOut));
                VecSound_Play(m_bufferPtr[0], 3.5f, 0.6f); m_showBtn = true;
            } else {
                m_tween.set(tweeny::from(m_posX2).to(m_posX1).during(850).via(tweeny::easing::exponentialOut));
                VecSound_Play(m_bufferPtr[0], 3.5f, 0.6f);
            } m_sliderTimer.restart();
        });
    //-------------//
        e.add([this]() -> bool {
            if (m_tween.isActive()) {
                auto v = m_tween.update<float>();
                auto p = getPos(); 
                setPos(Vector2f(v, p.y));
                updateAABB();
                for (auto b : m_btn) {
                    p = b->getPos();
                    b->setPos(Vector2f(v, p.y));
                    b->updateAABB();
                }
                if (!isHover() && !m_tween.isActive()) { m_showBtn = false; }
            } for (auto i = 0; i < m_nbBtn; i++) {
                if (m_btnTweens[i].isActive()) {
                    auto s = m_btnTweens[i].update<float>();
                    m_btn[i]->getSpr().setScale(s, s);
                    m_btn[i]->updateAABB();
                }
            } return false;
        });
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    int SideToolUI::getMode() { return m_mode; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    SideToolUI::~SideToolUI()
    { for (auto b : m_btn) { delete b; } }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void SideToolUI::update(Vector2f mpos) {
        if (m_sliderTimer.getElapsedTime().asSeconds() > 0.15f)
        { Panel::update(mpos); } if (m_isHover)
        { for (auto b : m_btn) { b->update(mpos); } }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void SideToolUI::click(bool b) {
        if (m_isHover) {
            for (auto btn : m_btn) { btn->click(b); }
        }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void SideToolUI::draw(RenderWindow& w) {
        Panel::draw(w);
        if (m_showBtn) {
            for (auto b : m_btn) { b->draw(w); }
        }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    GameState::GameState(StateManager& state_man, ResourceManager& res_man , RenderWindow& window, const bool replace)
    : State(state_man, res_man, window, replace), m_gameSpeed(1.f) {
    //-------------//
        m_background_color = Color(31, 68, 112, 255);
        InitWorld(0.f, 18.f, 1.f / 80.f, 7, 3);
        m_camera.init(&m_window, WIN_DIMS, WIN_CENTER);
        m_lastEntity = EntityName::PhysicRect;
    //-------------//
        m_blackScreen.setSize(WIN_DIMS);
        m_blackScreenCol = Color::Black;
        m_blackScreen.setFillColor(m_blackScreenCol);
        m_blackScreenTween.set(tweeny::from(255.f).to(0.f).during(800).via(tweeny::easing::quadraticOut));
    //-------------//
        m_res_man.load<Font>("Font", "fnt/Font.ttf");
        m_res_man.load<Texture>("Barrel1", "gfx/Barrel1.png");
        m_res_man.load<Texture>("Barrel2", "gfx/Barrel2.png");
        m_res_man.load<Texture>("Barrel3", "gfx/Barrel3.png");
        m_res_man.load<Texture>("Barrel4", "gfx/Barrel4.png");
        m_res_man.load<Texture>("Viseur", "gfx/Viseur.png");
        m_res_man.load<Texture>("Saw", "gfx/Saw.png");
        m_res_man.load<Texture>("Grab", "gfx/gui/Grab.png");
        m_res_man.load<Texture>("Link", "gfx/gui/Link.png");
        m_res_man.load<Texture>("Select", "gfx/gui/Select.png");
        m_res_man.load<SoundBuffer>("Bubble", "sfx/Bubble.mp3");
        m_res_man.load<SoundBuffer>("Pop", "sfx/Pop1.wav");
        m_res_man.load<SoundBuffer>("Hit", "sfx/Hit.wav");
        m_res_man.load<SoundBuffer>("UIpop1", "sfx/Pop2.wav");
        m_res_man.load<SoundBuffer>("UIpop2", "sfx/Pop3.wav");
        m_res_man.load<SoundBuffer>("woosh", "sfx/Woosh.mp3");
        m_res_man.load<SoundBuffer>("Sw_On", "sfx/SwitchOn.mp3");
        m_res_man.load<SoundBuffer>("Sw_Off", "sfx/SwitchOff.mp3");
    //-------------//
        m_bubbleSound.setBuffer(*res_man.get<SoundBuffer>("Bubble"));
        m_popSound.setBuffer(*res_man.get<SoundBuffer>("Pop"));
        m_bubbleSound.setVolume(40.f); m_popSound.setVolume(30.f);
        VevSound_SetMaxLimit(100);
    //-------------//
        m_player = new Ragdoll(Vector2f(600.f, 300.f));
        //auto playerChest = m_player->getM(1);
        //playerChest->getBody()->SetFixedRotation(true);
        //playerChest->getBody()->SetType(b2_staticBody);
    //-------------//
        //m_robot = new Robot(Vector2f(0.f,-500.f));
    //-------------//
        m_sideToolUI = new SideToolUI(m_gameEvery, res_man);
    //-------------//
        /*m_weight = NewRect(Vector2f(85.f, 60.f), Vector2f(0.f, -350.f));
        m_weight->getShape()->setOutlineColor(Color::Black);
        m_weight->getShape()->setOutlineThickness(2.5f);
        m_weight->getShape()->setFillColor(Color(50, 50, 50, 255));
        m_weight->getBody()->SetAngularDamping(2.f);
        m_weight->getBody()->SetGravityScale(2.f);
        m_weight->getFixture()->SetFriction(100.f);
        m_weight->getFixture()->SetDensity(30.f);
        m_weight->getBody()->ResetMassData();*/
    //-------------//
        m_paddle = NewRect(Vector2f(1000.f, 20.f), Vector2f(2000.f, -350.f));
        m_paddle->getShape()->setOutlineColor(Color::Black);
        m_paddle->getShape()->setOutlineThickness(1.5f);
        J[0] = NewLink(m_paddle->getBody(), Vector2f(1550.f, -350.f), Vector2f(1550.f, -350.f), 0.5f, 0.15f);
        J[1] = NewLink(m_paddle->getBody(), Vector2f(2450.f, -350.f), Vector2f(2450.f, -350.f), 0.5f, 0.15f);
        J[0]->SetMinLength(0.f); J[0]->SetMaxLength(sf_to_b2(500.f));
        J[1]->SetMinLength(0.f); J[1]->SetMaxLength(sf_to_b2(500.f));
        m_line[0].setThickness(2.5f); m_line[1].setThickness(2.5f);
    //-------------//
        Texture* ViseurTex = res_man.get<Texture>("Viseur");
        m_viseur.setTexture(*ViseurTex);
        m_viseur.setOrigin(((Vector2f)ViseurTex->getSize())/2.f);
        m_viseur.setPosition(WIN_CENTER);
        m_viseur.setScale(0.4f, 0.4f);
        m_viseur.setColor(Color(255, 255, 255, 150));
    //-------------//
        Vector2f GroundPos(WIN_W/2, WIN_H + 50.f);
        Vector2f GroundDims(18000.f, 100.f);
        m_ground = NewRect(GroundDims, GroundPos, b2_staticBody);
        m_ground->getShape()->setOutlineColor(Color::Black);
        m_ground->getShape()->setOutlineThickness(4.f);
        m_ground->getShape()->setFillColor(Color(127 , 127, 76));
    //-------------//
        m_queryCB = [this](FloatRect& z, Collider* c, b2Fixture* f) -> bool {
            Vector2f pos(z.left + z.width/2, z.top + z.height/2);
            if (c->getType() == b2_dynamicBody && !c->isSensor()) {
                if (c->testPoint(pos)) {
                    m_dragJoint.setObj(c->getBody(), pos);
                    m_bubbleSound.play(); return false;
                }
            } return true;
        };
    //-------------//
        m_creditText.setFont(*m_res_man.get<Font>("Font"));
        m_creditText.setCharacterSize(20U);
        m_creditText.setString("Made by : _Mahorin_");
        m_creditText.setPosition(WIN_W - 260, WIN_H - 40);
        m_creditText.setLetterSpacing(3.f);
        m_creditText.setOutlineColor(Color::Black);
        m_creditText.setOutlineThickness(3.f);
    //-------------//
        m_runtimeText.setFont(*m_res_man.get<Font>("Font"));
        m_runtimeText.setCharacterSize(20U);
        m_runtimeText.setString("ms = 0");
        m_runtimeText.setPosition(19, WIN_H - 40);
        m_runtimeText.setLetterSpacing(3.f);
        m_runtimeText.setOutlineColor(Color::Black);
        m_runtimeText.setOutlineThickness(3.f);
    //-------------//
        m_entityEvery.set(0.001f);
        m_gameEvery.set(0.001f);
        m_gameEvery.add([this]() -> bool {
            if (m_camZoomTween.isActive()) {
                if (m_dragJoint.isActive()) { m_dragJoint.target(m_camera.getMouse()); }
                m_camera.setZoom(m_camZoomTween.update<float>());
            } if (m_slowMotionTween.isActive()) {
                m_gameSpeed = m_slowMotionTween.update<float>();
            } if (m_blackScreenTween.isActive()) {
                m_blackScreenCol.a = (uint8_t)m_blackScreenTween.update<float>();
                m_blackScreen.setFillColor(m_blackScreenCol);
            } return false;
        });
    //-------------//
        m_camera.enableTarget();
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    GameState::~GameState() {
        Print("Destroy The Game");
        m_ground->destroy(m_ground);
        m_player->destroy(m_player);
        PhysicWorld->DestroyJoint(J[0]);
        PhysicWorld->DestroyJoint(J[1]);
        m_paddle->destroy(m_paddle);
        //m_weight->destroy(m_weight);
        VecSound_SuprAll();
        delete m_sideToolUI;
        unload(); reset();
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void GameState::unload() {
        m_res_man.unload<SoundBuffer>("Bubble");
        m_res_man.unload<SoundBuffer>("Pop");
        m_res_man.unload<SoundBuffer>("Hit");
        m_res_man.unload<SoundBuffer>("UIpop1");
        m_res_man.unload<SoundBuffer>("UIpop2");
        m_res_man.unload<SoundBuffer>("woosh");
        m_res_man.unload<SoundBuffer>("Sw_On");
        m_res_man.unload<SoundBuffer>("Sw_Off");
        m_res_man.unload<SoundBuffer>("Fart");
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void GameState::reset(bool anim) {
        for (auto& e : m_entityLst) { e->supr(anim); }
        if (!anim) { m_entityLst.clear(); m_entityEvery.call(); }
        for (auto& h : m_ragdollLst) { h->destroy(h); }
        m_ragdollLst.clear();
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void GameState::addBarrel(Vector2f pos, float size) {
        auto t = m_res_man.get<Texture>("Barrel"+to_string(Rdm(1, 4)));
        auto sb = m_res_man.get<SoundBuffer>("Hit");
        auto b = new Barrel(pos, t, size);
        b->setEvery(&m_entityEvery); b->setSoundBuffer(sb);
        m_entityLst.push_back(b); m_lastEntity = EntityName::Barrel;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void GameState::addBox(Vector2f pos, float size) {
        auto sb = m_res_man.get<SoundBuffer>("Hit");
        auto r = new PhysicRect(pos, Vector2f(35.f, 35.f) * size);
        r->setEvery(&m_entityEvery); r->setSoundBuffer(sb);
        m_entityLst.push_back(r); m_lastEntity = EntityName::PhysicRect;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void GameState::addSaw(Vector2f pos, float size) {
        auto t = m_res_man.get<Texture>("Saw");
        auto sb = m_res_man.get<SoundBuffer>("Hit");
        auto s = new Saw(pos, t, size);
        s->setEvery(&m_entityEvery); s->setSoundBuffer(sb);
        m_entityLst.push_back(s); m_lastEntity = EntityName::Saw;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void GameState::pause()
    { Print("Pause Game"); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void GameState::resume()
    { Print("Resume Game"); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void GameState::handleEvents(Event& event) {
    //-------------//
        switch (event.type) {
        //-------------//
            case Event::Closed:
                m_state_man.quit();
                break;
        //-------------//
            case Event::KeyPressed:
                switch (event.key.code) {
                //-------------//
                    case Keyboard::Escape:
                        unload(); m_state_man.quit(); break;
                //-------------//
                    case Keyboard::BackSpace:
                        m_state_man.lastState(); break;
                //-------------//
                    case Keyboard::R:
                        reset(true); break;
                //-------------//
                    case Keyboard::O:
                        m_camera.shake(); break;
                //-------------//
                    case Keyboard::C:
                        addBox(m_camera.getMouse()); break;
                //-------------//
                    case Keyboard::V:
                        addSaw(m_camera.getMouse()); break;
                //-------------//
                    case Keyboard::B:
                        addBarrel(m_camera.getMouse()); break;
                //-------------//
                    case Keyboard::S:
                        m_slowMotionTween.set(tweeny::from(m_gameSpeed).to(0.15f).during(600).via(tweeny::easing::exponentialOut));
                        m_bubbleSound.setPitch(0.5f); m_popSound.setPitch(0.5f);
                        break;
                //-------------//
                    case Keyboard::H:
                        {auto h = new Ragdoll(m_camera.getMouse());
                        h->setDir(RdmSign());
                        h->setActive(false);
                        m_ragdollLst.push_back(h);}
                        break;
                //-------------//
                } break;
        //-------------//
            case Event::KeyReleased:
                switch (event.key.code) {
                //-------------//
                    case Keyboard::S:
                        m_slowMotionTween.set(tweeny::from(m_gameSpeed).to(1.f).during(800).via(tweeny::easing::exponentialOut));
                        m_bubbleSound.setPitch(1.f); m_popSound.setPitch(1.f);
                        break;
                //-------------//
                } break;
        //-------------//
            case Event::MouseButtonPressed:
                switch (event.mouseButton.button) {
                //-------------//
                    case Mouse::Right:
                        m_camera.setDragMode(true);
                        break;
                //-------------//
                    case Mouse::Left:
                        m_sideToolUI->click(true);
                        break;
                //-------------//
                } break;
        //-------------//
            case Event::MouseMoved:
                if (m_dragJoint.isActive())
                { m_dragJoint.target(m_camera.getMouse()); }
                m_camera.updateDrag(event.mouseMove);
                break;
        //-------------//
            case Event::MouseButtonReleased:
                switch (event.mouseButton.button) {
                //-------------//
                    case Mouse::Right:
                        m_camera.setDragMode(false);
                        break;
                //-------------//
                    case Mouse::Left:
                        m_sideToolUI->click(false);
                        if (m_dragJoint.isActive())
                        { m_popSound.play(); m_dragJoint.unSet(); }
                        break;
                //-------------//
                } break;
        //-------------//
            case Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
                    float z = m_camera.getZoom();
                    if (event.mouseWheelScroll.delta > 0)
                    { m_camZoomTween.set(tweeny::from(z).to(z * 0.7f).during(1500).via(tweeny::easing::circularOut)); }
                    else if (event.mouseWheelScroll.delta < 0)
                    { m_camZoomTween.set(tweeny::from(z).to(z * 1.3f).during(1500).via(tweeny::easing::circularOut)); }
                } break;
        //-------------//
        }
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void GameState::update(float dt) {
    //-------------//
        m_sideToolUI->update((Vector2f)Mouse::getPosition(m_window));
        m_player->update(dt, m_camera.getMouse(), m_gameSpeed);
        m_line[0].setPoints(b2_to_sf(J[0]->GetAnchorA()), b2_to_sf(J[0]->GetAnchorB()));
        m_line[1].setPoints(b2_to_sf(J[1]->GetAnchorA()), b2_to_sf(J[1]->GetAnchorB()));
    //-------------//
        //m_robot->update(dt);
        m_paddle->update();
        //m_weight->update();
    //-------------//
        if (m_runtimeClock.getElapsedTime() > seconds(0.5)) {
            m_runtimeText.setString("ms = " + to_string((short)(dt*1000)));
            m_runtimeClock.restart();
        }
    //-------------//
        if (m_gameSpeed >= 0.99f) { m_gameSpeed = 1.f; } else
        if (m_gameSpeed <= 0.151f) { m_gameSpeed = 0.15f; }
    //-------------//
        if (m_entityLst.size() > 0) {
            auto e = m_entityLst.at(m_entityLst.size()-1);
            m_camera.target(e->getPos());
        }
    //-------------//
        if (Keyboard::isKeyPressed(Keyboard::U)) {
            float a = Rdm(0.f, b2_pi*2), d = Rdm(100.f, 400.f);
            Vector2f pos(m_camera.getMouse().x + cosf(a) * d, m_camera.getMouse().y + sinf(a) * d);
            switch (m_lastEntity) {
                case EntityName::Saw:
                    addSaw(pos); break;
                case EntityName::Barrel:
                    addBarrel(pos); break;
                case EntityName::PhysicRect:
                    addBox(pos); break;
            }
        } if (Keyboard::isKeyPressed(Keyboard::E)) {
        //-------------//
            if (m_mouseAngleSpeed < 0.f)
            { m_mouseAngleSpeed = 0.f; } else
            if (m_mouseAngleSpeed > 30.f)
            { m_mouseAngleSpeed = 30.f; }
        //-------------//
            m_dragJoint.rotate(m_mouseAngleSpeed);
            m_dragJoint.setAngleMode(true);
            m_mouseAngleSpeed += 6.5f * dt;
        //-------------//
        } else if (Keyboard::isKeyPressed(Keyboard::Q)) {
        //-------------//
            if (m_mouseAngleSpeed > 0.f)
            { m_mouseAngleSpeed = 0.f; } else
            if (m_mouseAngleSpeed < -30.f)
            { m_mouseAngleSpeed = -30.f; }
        //-------------//
            m_dragJoint.rotate(m_mouseAngleSpeed);
            m_dragJoint.setAngleMode(true);
            m_mouseAngleSpeed -= 6.5f * dt;
        //-------------//
        } else { m_mouseAngleSpeed = 0.f; }
    //-------------//
        for (int i = 0; i < m_entityLst.size(); i++) {
            SpawnEntity* e = m_entityLst[i];
            e->update(dt); e->setSoundSpeed(m_gameSpeed);
            if (Keyboard::isKeyPressed(Keyboard::X)) {
                if (e->getCorps()->testPoint(m_camera.getMouse())) {
                    if (m_dragJoint.sameBody(e->getCorps()->getBody()))
                    { m_dragJoint.unSet(); }
                    if (!e->is_supr(true)) { e->supr(true); }
                }
            }
            if (e->is_supr()) {
                if (m_dragJoint.sameBody(e->getCorps()->getBody()))
                { m_dragJoint.unSet(); } e->destroy();
                m_entityLst.erase(m_entityLst.begin() + i);
                i--;
            }
        }
    //-------------//
        for (auto* h : m_ragdollLst) { h->update(dt, m_camera.getMouse(), m_gameSpeed); }
    //-------------//
        m_dragJoint.update();
        m_camera.update(dt * m_gameSpeed);
        VecSound_Update();
        m_entityEvery.update(dt * m_gameSpeed);
        m_gameEvery.update(dt);
        UpdateWorld(dt, m_gameSpeed);
    //-------------//
        if (!m_dragJoint.isActive() && Mouse::isButtonPressed(Mouse::Left)) {
            setFloatRect(m_queryRect, m_camera.getMouse(), Vector2f(10.f, 10.f));
            QueryWorldZone(m_queryRect, m_queryCB);
        }
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void GameState::draw() {
    //-------------//
        m_window.clear(m_background_color);
        m_camera.set();
            m_ground->draw(m_window);
    //-------------//
            for (auto* e : m_entityLst) { e->draw(m_window); }
            for (auto* h : m_ragdollLst) { h->draw(m_window); }
            m_window.draw(m_line[0]);
            m_window.draw(m_line[1]);
    //-------------//
            m_player->draw(m_window);
            //m_robot->draw(m_window);
            m_paddle->draw(m_window);
            //m_weight->draw(m_window);
            m_dragJoint.draw(m_window);
        m_camera.unset();
            m_sideToolUI->draw(m_window);
            m_window.draw(m_viseur);
            m_window.draw(m_creditText);
            m_window.draw(m_runtimeText);
            m_window.draw(m_blackScreen);
        m_window.display();
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//