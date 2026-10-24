//==============================================================//
#include <main.hpp>
//==============================================================//
using namespace sf; using namespace std;
//==============================================================//
float GameSpeed = 1;
bool SlowMotion = false;
//==============================================================//
void VecSound_Play(SoundBuffer* bf, float volume = 50.f, float pitch = 1.f) {
    if (Sounds_Vector.size() < 70 && volume > 0.5f) {
    Sound* snd = new Sound(*bf);
    if (SlowMotion) { pitch -= 0.5f; }
    if (pitch < 0.1f) { pitch = 0.1f; }
    snd->setPitch(pitch);
    snd->setVolume(volume); snd->play();
    Sounds_Vector.push_back(snd);
    }
}
//-----------------------------//
void VecSound_Update() {
    if (Sounds_Vector.size() > 0) {
        for (auto it = Sounds_Vector.rbegin(); it != Sounds_Vector.rend(); ++it) {
            if ((*it)->getStatus() == SoundSource::Stopped)
            { delete* it; Sounds_Vector.erase(next(it).base()); }
        }
    }
}
//==============================================================//
class Particule {
public:
//~~~~~~~~~~~~~~~~//
    Particule(Vector2f pos, Vector2f vel, float size, float life)
    : Pos(pos), Vel(vel), Size(size), Life(life) {
        //===========//
        Timer.restart();
        isSupr = false;
        Grav = 0;
        Shape = new CircleShape;
        //===========//
    }
//~~~~~~~~~~~~~~~~//
    void update(float dt) {
        setSize(Size);
        dt *= GameSpeed;
        Pos += Vel * dt;
        Shape->setPosition(Pos);
        if (Grav > 0) { Vel.y += Grav * 100.f * dt; }
        if (Timer.getElapsedTime().asSeconds()>Life) 
        { if (Size > 0) { Size -= 10.f * dt; } else { destroy(); } }
    }
//~~~~~~~~~~~~~~~~//
    void setColor(Color col) { Shape->setFillColor(col); }
//~~~~~~~~~~~~~~~~//
    void setGravity(float g) { Grav = g; }
//~~~~~~~~~~~~~~~~//
    void destroy() { delete Shape; isSupr = true; }
//~~~~~~~~~~~~~~~~//
    bool getSupr() { return isSupr; }
//~~~~~~~~~~~~~~~~//
    void draw(RenderWindow& window) { if (!isSupr) { window.draw(*Shape); } }
//~~~~~~~~~~~~~~~~//
    CircleShape* getShape() { return Shape; }
//~~~~~~~~~~~~~~~~//
private:
//~~~~~~~~~~~~~~~~//
    float Grav;
    float Size;
    float Life;
    bool isSupr;
    Clock Timer;
    Vector2f Pos;
    Vector2f Vel;
    CircleShape* Shape;
    void setSize(float s) { Shape->setRadius(s); Shape->setOrigin(s, s); }
//~~~~~~~~~~~~~~~~//
};
//==============================================================//
Particule* NewParticule(Vector2f pos, Vector2f vel, float size = 3.f, float life = 3.f, bool PushBack = true) {
    Particule* p = new Particule(pos, vel, size, life);
    if (PushBack) { Particule_Vector.push_back(p); } else
    { Particule_Vector.insert(Particule_Vector.begin(), p); }
    return p;
}
//-----------------------------//
void UpdateParticules(float dt) {
    for (int i = 0; i < Particule_Vector.size(); i++) {
        Particule_Vector[i]->update(dt);
        if (Particule_Vector[i]->getSupr()) {
            delete Particule_Vector[i];
            Particule_Vector.erase(Particule_Vector.begin() + i);
            i--;
        }
    }
}
//==============================================================//
class PhysicsBox {
public:
//~~~~~~~~~~~~~~~~//
    PhysicsBox(Vector2f pos, float side) {
        //===========//
        corps = NewBoxCollider(side, pos);
        b2Fixture* f = corps->getFixture();
        f->SetDensity(Rdm(1.f, 50.f));
        f->SetFriction(6.f);
        f->SetRestitution(0.65f);
        b2Body* b = corps->getBody();
        b->SetAngularDamping(1.5f);
        corps->setUserData(reinterpret_cast<uintptr_t>(this));
        //===========//
        Shape.setSize(Vector2f(side, side));
        Shape.setFillColor(Color::Transparent);
        Shape.setOutlineColor(Color::Black);
        Shape.setOutlineThickness(4.f);
        Shape.setOrigin(side/2, side/2);
        Shape.setScale(0.f, 0.f);
        //===========//
        Gradient_Transform.setOrigin(side/2, side/2);
        Gradient_Transform.setScale(0.f, 0.f);
        Gradient.setPrimitiveType(Quads);
        Gradient.resize(4);
        //===========//
        Gradient[0].position = Vector2f(0.f, 0.f);
        Gradient[1].position = Vector2f(side, 0.f);
        Gradient[2].position = Vector2f(side, side);
        Gradient[3].position = Vector2f(0.f, side);
        //===========//
        Gradient[0].color = HSVtoRGB(Rdm(0.f, 360.f), 1.f, 1.f);
        Gradient[1].color = HSVtoRGB(Rdm(0.f, 360.f), 1.f, 1.f);
        Gradient[2].color = HSVtoRGB(Rdm(0.f, 360.f), 1.f, 1.f);
        Gradient[3].color = HSVtoRGB(Rdm(0.f, 360.f), 1.f, 1.f);
        //===========//
        SpawnTween = tweeny::from(0.f, -50.f).to(1.f, 0.f).during(1500).via(tweeny::easing::elasticOut);
        accumulatedTime = 0.f; IsTweenActive = true;
        //===========//
        corps->Begin_CB = [this](Collider* collider, b2Contact* contact) -> void {
            float Volume = Clip(Map_Range(corps->getTotalVel(), 0.f, 4000.f, 0.f, 100.f), 0.f, 100.f);
            float Pitch = Map_Range(corps->getDensity(), 1.f, 50.f, -0.38f, 0.38f);
            playSound(Volume, (1-Pitch) + Rdm(-0.2f, 0.2f));
        };
        //===========//
    }
//~~~~~~~~~~~~~~~~//
    void update(float dt) {
        float a = corps->getAngle();
        Shape.setPosition(corps->getPos());
        Gradient_Transform.setPosition(corps->getPos());
        if (IsTweenActive) {
        accumulatedTime += dt * GameSpeed;
        while (accumulatedTime >= 0.001f) {
            array<float, 2> v = SpawnTween.step(1);
            accumulatedTime -= 0.001f;
            Shape.setScale(v[0], v[0]);
            Gradient_Transform.setScale(v[0], v[0]);
            Shape.setRotation(a + v[1]);
            Gradient_Transform.setRotation(a + v[1]);
        }
        IsTweenActive = !(SpawnTween.progress()==1);
        } else { Shape.setRotation(a); Gradient_Transform.setRotation(a); }
    }
//~~~~~~~~~~~~~~~~//
    void destroy() {
        corps->destroy();
        delete corps;
    }
//~~~~~~~~~~~~~~~~//
    void draw(RenderWindow& window) { window.draw(Gradient, Gradient_Transform.getTransform()); window.draw(Shape); }
//~~~~~~~~~~~~~~~~//
    void playSound(float v = 100.f, float p = 1.f) {
        float t = SoundClock.getElapsedTime().asSeconds();
        if (t > 0.25f) { VecSound_Play(&buffer4, v, p); SoundClock.restart(); }
    }
//~~~~~~~~~~~~~~~~//
    Collider* corps;
    const string Type = "GradientBox";
//~~~~~~~~~~~~~~~~//
private:
//~~~~~~~~~~~~~~~~//
    RectangleShape Shape;
    Clock SoundClock;
    VertexArray Gradient;
    Transformable Gradient_Transform;
    tweeny::tween<float, float> SpawnTween;
    float accumulatedTime; bool IsTweenActive;
//~~~~~~~~~~~~~~~~//
};
//==============================================================//
void AddBox(float x, float y, float size = Rdm(25.f, 45.f)) {
    PhysicsBox* b = new PhysicsBox(Vector2f(x, y), size);
    Boxes_Vector.push_back(b);
}
//==============================================================//
class MJ_Drawable {
public:
//~~~~~~~~~~~~~~~~//
    MJ_Drawable() {
        shapeA.setRadius(4.5f);
        shapeB.setRadius(4.5f);
        shapeA.setOutlineThickness(2.f);
        shapeB.setOutlineThickness(2.f);
        shapeA.setFillColor(Color::Yellow);
        shapeB.setFillColor(Color::Cyan);
        shapeA.setOutlineColor(Color::Black);
        shapeB.setOutlineColor(Color::Black);
        shapeA.setOrigin(4.5f, 4.5f);
        shapeB.setOrigin(4.5f, 4.5f);
        Color rect_col(250,250,250,200);
        rectLine.setFillColor(rect_col);
        rectLine.setOrigin(Vector2f(0, 3.5f/2));
        rectLine.setOutlineColor(Color::Black);
        rectLine.setOutlineThickness(2.f);
    }
//~~~~~~~~~~~~~~~~//
    void draw(RenderWindow& window, MouseJoint& mj) {
        if (mj.isActive()) {
            Vector2f posA = mj.getAnchor(1);
            Vector2f posB = mj.getAnchor(2);
            shapeA.setPosition(posA); shapeB.setPosition(posB);
            float a = Math_Angle(posA, posB);
            float d = Math_Dist(posA, posB);
            rectLine.setRotation(toDeg(a));
            rectLine.setSize(Vector2f(d, 3.5f));
            rectLine.setPosition(posA);
            window.draw(rectLine);
            window.draw(shapeB);
            window.draw(shapeA);
        }
    }
//~~~~~~~~~~~~~~~~//
private:
//~~~~~~~~~~~~~~~~//
    CircleShape shapeA;
    CircleShape shapeB;
    RectangleShape rectLine;
//~~~~~~~~~~~~~~~~//
};
//==============================================================//
void resetGame() {
    for (int i = 0; i < Boxes_Vector.size(); i++) {
        PhysicsBox* Box = Boxes_Vector[i];
        Box->destroy(); delete Box;
        Boxes_Vector.erase(Boxes_Vector.begin() + i);
        i--;
    }
    for (int i = 0; i < Ragdolls_Vector.size(); i++) {
        Ragdoll* H = Ragdolls_Vector[i];
        H->destroy(); delete H;
        Ragdolls_Vector.erase(Ragdolls_Vector.begin() + i);
        i--;
    }
}
//==============================================================//
void computeFps(Time ts, int& fps) {
    ComputeFpsTimer += ts; FrameCount += 1;
    if (ComputeFpsTimer >= seconds(1.0f)) {
        fps = FrameCount;
        ComputeFpsTimer -= seconds(1.0f);
        FrameCount = 0;
    }
}
//==============================================================//
int main() {
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
    // Crée la fenêtre SFML
    ContextSettings MySettings; MySettings.antialiasingLevel = 16;
    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT), "Demo SFML Box2d",Style::Titlebar | Style::Close, MySettings);
    Image icon; icon.loadFromFile("res/gcc/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setFramerateLimit(144); Color BackgroundCol(31, 68, 112, 255);
    window.setPosition(Vector2i(100, 35));
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
    // Crée le monde Box2D et une Caméra
    InitWorld(0.f, 13.f, 1.f / 100.f, 8, 3);
    View Camera(Vector2f(WIN_WIDTH/2, WIN_HEIGHT/2), Vector2f(WIN_WIDTH, WIN_HEIGHT));
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
    // Crée le Sol
    Vector2f GroundPos(WIN_WIDTH/2, WIN_HEIGHT + 50.f); Vector2f GroundDims(18000.f, 100.f);
    ShapedCollider* Ground = NewRect(GroundDims, GroundPos, b2_staticBody);
    Ground->getShape()->setOutlineColor(Color::Black);
    Ground->getShape()->setOutlineThickness(4.f);
    Ground->getShape()->setFillColor(Color(127 , 127, 76));
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
    // Initialise des sons à l'intèrieur des buffers
    buffer1.loadFromFile("res/sfx/Brick.wav");
    buffer2.loadFromFile("res/sfx/Pop.wav");
    buffer3.loadFromFile("res/sfx/Bubble.mp3");
    buffer4.loadFromFile("res/sfx/Hit.wav");
    Sound Bubble_Snd(buffer3); Bubble_Snd.setVolume(40.f);
    Sound Pop_Snd(buffer2); Pop_Snd.setVolume(30.f);
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
    //Music music;
    //if (!music.openFromFile("res/msc/CalmSong.mp3")) { return -1; }
    //music.play(); music.setLoop(true); music.setVolume(2.f);
    //music.setPitch(0.8);
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
    font.loadFromFile("res/fnt/font.ttf");
    Text Guide("ESC  :  Quitte le programme \nR  :  Reset la scene \nQ  :  Retire l'objet a l'emplacement de la souris \nW  :  Fait spawn des objets \nClic Droit  :  Fait spawn 1 objet \nClic Gauche  :  Attrape un objet \nS  :  Active le SlowMotion", font, 20U);
    Guide.setPosition(19, 18);
    Guide.setLetterSpacing(3.f);
    Guide.setLineSpacing(1.4f);
    Guide.setOutlineColor(Color::Black);
    Guide.setOutlineThickness(3.f);
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
    Text TheDev("Made by : _Mahorin_", font, 20U);
    TheDev.setPosition(WIN_WIDTH - 260, WIN_HEIGHT - 40);
    TheDev.setLetterSpacing(3.f);
    TheDev.setOutlineColor(Color::Black);
    TheDev.setOutlineThickness(3.f);
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
    Text RunTime("", font, 20U);
    RunTime.setPosition(19, WIN_HEIGHT - 40);
    TheDev.setLetterSpacing(3.f);
    RunTime.setOutlineColor(Color::Black);
    RunTime.setOutlineThickness(3.f);
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
    float TweenerAccumulatedTime = 0.f;
    tweeny::tween<float, float> CamPosTween;
    tweeny::tween<float, float> CamZoomTween;
    bool IsCamPosTweenActive = false, IsCamZoomTweenActive = false;
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
    // Initialise des variable et crée un MouseJoint
    Mouse mouse; Clock FrameClock; int FPS;
    MouseJoint MJ; MJ_Drawable MJ_Draw;
    FloatRect QueryRect; Ragdoll Human(Vector2f(WIN_WIDTH/2, WIN_HEIGHT - 105));
    auto Query_CB = [&Bubble_Snd, &MJ](FloatRect& z, Collider* c, b2Fixture* f) -> bool {
        Vector2f pos(z.left + z.width/2, z.top + z.height/2);
        if (c->getType()==b2_dynamicBody) {
            if (c->testPoint(pos)) { Bubble_Snd.play(); MJ.setObj(c->getBody(), pos); return false; }
        } return true;
    };
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
    Vector2f circPos(100.f, 100.f);
    ShapedCollider* CircleCollider = NewRect(Vector2f(280.f, 50.f), circPos);
    CircleCollider->getShape()->setOutlineThickness(3.f);
    CircleCollider->getShape()->setOutlineColor(Color::Black);
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = sf_to_b2(circPos);
    b2Body* ghost_body = PhysicWorld->CreateBody(&bodyDef);
    NewBearing(ghost_body, CircleCollider->getBody(), Vector2f(0.f, 100.f));
    //CircleShape* sh = static_cast<CircleShape*>(CircleCollider->getShape());
    //sh->setPointCount(50);
    CircleCollider->getFixture()->SetRestitution(0.5f);
//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
    // Boucle principale
    while (window.isOpen()) {
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        // Récupère la position de la souris à l'écran
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        // Appel le gestionnaire d'évènements
        Event event; while (window.pollEvent(event)) {
            if (event.type == Event::Closed) { 
                window.close();
            } else if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                    case Keyboard::Escape:
                        window.close(); break;
                    case Keyboard::R:
                        resetGame(); break;
                    case Keyboard::C:
                        AddBox(mousePos.x, mousePos.y); break;
                    case Keyboard::H:
                        Ragdoll* h = new Ragdoll(mousePos);
                        h->setDir(RdmSign());
                        Ragdolls_Vector.push_back(h);
                        break;
                }
            } else if (event.type == Event::MouseButtonPressed) {
                switch (event.mouseButton.button) {
                    case Mouse::Middle:
                        Vector2f center = Camera.getCenter();
                        CamPosTween = tweeny::from(center.x, center.y).to(mousePos.x, mousePos.y).during(1000).via(tweeny::easing::exponentialOut);
                        IsCamPosTweenActive = true; break;
                }
            } else if (event.type == Event::MouseMoved) {
                if (MJ.isActive()) { MJ.update(mousePos); }
            } else if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button==Mouse::Left && MJ.isActive()) { Pop_Snd.play(); MJ.unSet(); }
            } else if (event.type == Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
                    Vector2f s = Camera.getSize(); IsCamZoomTweenActive = true;
                    if (event.mouseWheelScroll.delta > 0)
                    { CamZoomTween = tweeny::from(s.x, s.y).to(s.x*0.8f, s.y*0.8f).during(1000).via(tweeny::easing::circularOut); }
                    else if (event.mouseWheelScroll.delta < 0)
                    { CamZoomTween = tweeny::from(s.x, s.y).to(s.x*1.2f, s.y*1.2f).during(1000).via(tweeny::easing::circularOut); }
                }
            }
        }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        // Gère la fonctionnalité du SlowMotion
        if (!SlowMotion && Keyboard::isKeyPressed(Keyboard::S)) {
            SlowMotion = true; GameSpeed = 0.15f;
            Bubble_Snd.setPitch(0.6f); Pop_Snd.setPitch(0.6f);
        } else if (SlowMotion && !Keyboard::isKeyPressed(Keyboard::S)) { 
            SlowMotion = false; GameSpeed = 1.0f;
            Bubble_Snd.setPitch(1.f); Pop_Snd.setPitch(1.f);
        }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        computeFps(TimeStep, FPS); RunTime.setString("fps = " + to_string(FPS));
        TimeStep = FrameClock.restart();
        float dt = TimeStep.asSeconds();
        dt = (dt > 0.2f) ? 0 : dt;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        // Gestions des Updates
        //CircleCollider->moveRot(0.f, 10.f);
        Human.update();
        VecSound_Update();
        UpdateParticules(dt);
        UpdateWorld(dt, 1);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        if (Keyboard::isKeyPressed(Keyboard::U)) {
            float a = Rdm(0.f, b2_pi*2), d = Rdm(50.f, 300.f);
            AddBox(mousePos.x + cosf(a) * d, mousePos.y + sinf(a) * d);
        }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        if (IsCamPosTweenActive || IsCamZoomTweenActive) {
            if (MJ.isActive()) { MJ.update(mousePos); }
            TweenerAccumulatedTime += TimeStep.asSeconds();
            while (TweenerAccumulatedTime >= 0.001f) {
                if (IsCamPosTweenActive) {
                    array<float, 2> point = CamPosTween.step(1);
                    Camera.setCenter(point[0], point[1]);
                    IsCamPosTweenActive = !(CamPosTween.progress()==1);
                }
                if (IsCamZoomTweenActive) {
                    array<float, 2> point = CamZoomTween.step(1);
                    Camera.setSize(point[0], point[1]);
                    IsCamZoomTweenActive = !(CamZoomTween.progress()==1);
                }
                TweenerAccumulatedTime -= 0.001f;
            }
        }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        // Met à jour et supprime les PhysicsBox
        for (int i = 0; i < Boxes_Vector.size(); i++) {
            PhysicsBox* Box = Boxes_Vector[i];
            Box->update(dt);
            if (Keyboard::isKeyPressed(Keyboard::Q)) {
                if (Box->corps->testPoint(mousePos)) {
                    if (MJ.sameBody(Box->corps->getBody())) { MJ.unSet(); }
                    Box->destroy(); delete Box;
                    Boxes_Vector.erase(Boxes_Vector.begin() + i);
                    i--;
                }
            }
        }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        // Gestion du clic gauche de la souris
        if (Mouse::isButtonPressed(Mouse::Left)) {
        //---------------//
            if (!MJ.isActive()) {
                setFloatRect(QueryRect, mousePos, Vector2f(10.f, 10.f));
                QueryZone(QueryRect, Query_CB);
            }
        //---------------//
        }/*else { 
        //---------------//
            float a = Rdm(0.f, 360.f), f = Rdm(10.f, 30.f);
            Vector2f vel(cosf(a) * f, sinf(a) * f);
            Vector2f pos(mousePos.x, mousePos.y);
            Particule* p = NewParticule(pos, vel, Rdm(2.f, 4.f), 0.f, false);
            p->setColor(HSVtoRGB(a, 0.8f, 1.0f));
            p->getShape()->setOutlineColor(Color::Black);
            p->getShape()->setOutlineThickness(1.5f);
        //---------------//
        }*/
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
        // Affichage des différents éléments à l'écran
        window.clear(BackgroundCol);
        Ground->draw(window);
        for (PhysicsBox* box : Boxes_Vector) { box->draw(window); }
        for (Ragdoll* h : Ragdolls_Vector) { h->draw(window); }
        Human.draw(window);
        CircleCollider->draw(window);
        MJ_Draw.draw(window, MJ);
        for (Particule* p : Particule_Vector) { p->draw(window); }
        window.setView(window.getDefaultView());
        window.draw(Guide);
        window.draw(TheDev);
        window.draw(RunTime);
        window.setView(Camera);
        window.display();
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    }
    return 0;
}
//==============================================================//