//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #pragma once
    #include <iostream>
    #include <random>
    #include <functional>
    #include <tweeny/tweeny.h>
    #include <globals.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    template <typename T> void PrintT(T t)
    { std::cout << std::to_string(t) << std::endl; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Print(std::string t);
    void PrintVec(sf::Vector2f vec);
    void PrintVec(sf::Vector2i vec);
    void PrintLine(char c = '-', uint8_t size = 40, char border = '\0');
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float sf_to_b2(const float v);
    float b2_to_sf(const float v);
    b2Vec2 sf_to_b2(const sf::Vector2f& v);
    sf::Vector2f b2_to_sf(const b2Vec2& v);
    b2AABB sf_to_b2(const sf::FloatRect& fr);
    sf::FloatRect b2_to_sf(const b2AABB& aabb);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float toDeg(float rad);
    float toRad(float deg);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    // maintient un angle entre 0 et 359
    float capRad(float rad);
    float capDeg(float deg);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    // maintient un angle entre -179 et 179
    float capRad2(float rad);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    bool CoinFlip(); int RdmSign();
    float Rdm(float min, float max);
    int Rdm(int min, int max);
//~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Math_Angle(float x1, float y1, float x2, float y2);
    float Math_Angle(sf::Vector2f p1, sf::Vector2f p2);
    float Math_Dist(float x1, float y1, float x2, float y2);
    float Math_Dist(sf::Vector2f p1, sf::Vector2f p2);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    int Sign(float v);
    float Clip(float v, float lower, float upper);
    float Damp(float v, float d, float dt);
    sf::Vector2f Damp(sf::Vector2f v, float d, float dt);
    void DampRef(float& v, float d, float dt);
    void DampRef(sf::Vector2f& v, float d, float dt);
    float Map_Range(float v, float fromLow, float fromHigh, float toLow, float toHigh);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    sf::Color HSVtoRGB(float H, float S, float V);
    sf::Color OKLABtoRGB(float L, float A, float B);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    sf::Color RdmColor(float satu = 1.f, float val = 1.f);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void setFloatRect(sf::FloatRect& fr, const sf::Vector2f& pos, const sf::Vector2f& size);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    sf::Vector2f AnchorVec(sf::Vector2f& pos1, sf::Vector2f& pos2);
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float CubicOutFunc(float x);
    float CubicInFunc(float x);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    sf::Vector2f IntersectTwoCircles(sf::Vector2f posA, sf::Vector2f posB, float r1, float r2, bool returnFisrt = true);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    // a1 = Target / a2 = Current
    float ShortestAngleDif(float a1, float a2);
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class PIDcontroller {
    //-------------//
    public:
        PIDcontroller(float P = 1.f, float I = 1.f, float D = 1.f);
        float update(float current, float target, float dt, bool angleMode = false);
        void setValueDerivativeMode(bool b = true);
        void setPID(float P, float I, float D);
        void setOutputLimit(float min, float max);
        void reset();
    //-------------//
        float m_gainP;
        float m_gainI;
        float m_gainD;
        float m_limitI;
    //-------------//
    private:
        float m_storedI;
        float m_lastError;
        float m_lastValue;
        float m_outputMin;
        float m_outputMax;
        bool m_valueDerivativeMode;
        bool m_dSwitch;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    using Every_CB = std::function<bool()>;
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class Every {
    //-------------//
    public:
        Every(float t = 1.f): m_step(t), m_timer(0) {}
    //-------------//
        void set(float t) { m_step = t; }
        void add(Every_CB cb) { m_callbacks.push_back(cb); }
        void clean() { m_callbacks.clear(); }
        void call() { m_callbacks.erase(std::remove_if(m_callbacks.begin(), m_callbacks.end(), [](const Every_CB& cb) { return cb(); }), m_callbacks.end()); }
    //-------------//
        void update(float dt) {
            m_timer += dt;
            while (m_timer >= m_step)
            { call(); m_timer -= m_step; }
        }
    //-------------//
    private:
        float m_step, m_timer;
        std::vector<Every_CB> m_callbacks;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    template<typename... Ts>
    class Tweener {
    //-------------//
    public:
        Tweener(): m_active(false) {}
    //-------------//
        void set(tweeny::tween<Ts...> t)
        { m_tween = t; m_active = true; }
    //-------------//
        bool isActive() { return m_active; }
    //-------------//
        template <typename T>
        T update() {
            auto v = m_tween.step(1);
            m_active = !(m_tween.progress()==1);
            return v;
        }
    //-------------//
    private:
        tweeny::tween<Ts...> m_tween;
		bool m_active;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class Catenary {
    //-------------//
    public:
        Catenary(int iterations = 10);
        void Solve(sf::Vector2f p1, sf::Vector2f p2, float l);
        sf::Vector2f getPoint(float t);
        void setIterations(int i);
        float getX(float t);
        float getY(float x);
        int8_t getMode();
        sf::Vector2f getMidP();
    //-------------//
    private:
        float m_a;
        float m_x;
        float m_y;
        float m_l;
        float m_c;
        int m_iter;
        bool m_isFlip;
        int8_t m_mode;
        sf::Vector2f m_p1;
        sf::Vector2f m_p2;
        sf::Vector2f m_midp;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//