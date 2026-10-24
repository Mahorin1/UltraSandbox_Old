//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #include <utils.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
    using namespace sf;
    using namespace std;
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    uniform_real_distribution<float> urdf;
    uniform_int_distribution<int> urdi;
    mt19937 Rdm_Generator;
    const float EPSILON = 1e-6;
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Print(string t) { cout << t << endl; }
    void PrintVec(Vector2f vec)
    { cout << "x = "+to_string(vec.x)+" / y = "+to_string(vec.y) << endl; }
    void PrintVec(Vector2i vec)
    { cout << "x = "+to_string(vec.x)+" / y = "+to_string(vec.y) << endl; }
    void PrintLine(char c, uint8_t size, char border) {
        string str = "" + border;
        for (uint8_t i = 0; i < size; i++)
        { str += c; } str += border;
        cout << str << endl;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float sf_to_b2(const float v) { return (v / B2SCALE); }
    float b2_to_sf(const float v) { return (v * B2SCALE); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2Vec2 sf_to_b2(const Vector2f& v)
    { return b2Vec2(v.x / B2SCALE, v.y / B2SCALE); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f b2_to_sf(const b2Vec2& v)
    { return Vector2f(v.x * B2SCALE, v.y * B2SCALE); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    b2AABB sf_to_b2(const FloatRect& fr) {
        b2AABB aabb;
        aabb.lowerBound.Set(fr.left / B2SCALE, fr.top / B2SCALE);
        aabb.upperBound.Set((fr.left + fr.width) / B2SCALE, (fr.top + fr.height) / B2SCALE);
        return aabb;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    FloatRect b2_to_sf(const b2AABB& aabb)
    { return FloatRect(aabb.lowerBound.x  * B2SCALE, aabb.lowerBound.y  * B2SCALE, (aabb.upperBound.x - aabb.lowerBound.x)  * B2SCALE, (aabb.upperBound.y - aabb.lowerBound.y)  * B2SCALE); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float toDeg(float rad)
    { return 180.f / b2_pi * rad; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float toRad(float deg)
    { return b2_pi / 180.f * deg; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float capRad(float rad) {
        rad = fmod(rad, PI2);
        if (rad < 0.0) { rad += PI2; }
        return rad;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float capDeg(float deg) {
        deg = fmod(deg, 360.0);
        if (deg < 0.0) { deg += 360.0; }
        return deg;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float capRad2(float rad) {
        rad = fmod(rad, PI2);
        rad = fmod(rad + PI2, PI2);
        if (rad > b2_pi) { rad -= PI2; }
        return rad;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    bool CoinFlip() { return (generate_canonical<float, 1>(Rdm_Generator) < 0.5); }
    int RdmSign() { return (CoinFlip()) ? -1 : 1; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Rdm(float min, float max) {
        urdf.param(uniform_real_distribution<float>::param_type(min, max));
        return urdf(Rdm_Generator);
    }
    int Rdm(int min, int max) {
        urdi.param(uniform_int_distribution<int>::param_type(min, max));
        return urdi(Rdm_Generator);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Math_Angle(float x1, float y1, float x2, float y2)
    { return atan2f(y2 - y1, x2 - x1); }
    float Math_Angle(Vector2f p1, Vector2f p2)
    { return atan2f(p2.y - p1.y, p2.x - p1.x); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Math_Dist(float x1, float y1, float x2, float y2)
    { return hypot(x2 - x1, y2 - y1); }
    float Math_Dist(Vector2f p1, Vector2f p2)
    { return hypot(p2.x - p1.x, p2.y - p1.y); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    int Sign(float v) { return (v > 0) ? 1 : ((v < 0) ? -1 : 0); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Clip(float v, float lower, float upper)
    { return max(lower, min(v, upper)); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Map_Range(float v, float fromLow, float fromHigh, float toLow, float toHigh) {
        float normalized = (v - fromLow) / (fromHigh - fromLow);
        float mapped = (normalized * (toHigh - toLow)) + toLow;
        return mapped;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Damp(float v, float d, float dt)
    { return v * (1.f - d * dt); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f Damp(Vector2f v, float d, float dt)
    { float damp = (1.f - d * dt); return Vector2f(v.x*=damp, v.y*=damp); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DampRef(float& v, float d, float dt)
    { v *= (1.f - d * dt); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void DampRef(Vector2f& v, float d, float dt)
    { float damp = (1.f - d * dt); v.x *= damp, v.y *= damp; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Color HSVtoRGB(float H, float S, float V) {
        int HueValue = static_cast<int>(H / 60.0f) % 6;
        float f = H / 60.0f - static_cast<float>(HueValue);
        float p = V * (1.0f - S);
        float q = V * (1.0f - f * S);
        float t = V * (1.0f - (1.0f - f) * S);
        switch (HueValue) {
            case 0: return Color(static_cast<Uint8>(V * 255), static_cast<Uint8>(t * 255), static_cast<Uint8>(p * 255));
            case 1: return Color(static_cast<Uint8>(q * 255), static_cast<Uint8>(V * 255), static_cast<Uint8>(p * 255));
            case 2: return Color(static_cast<Uint8>(p * 255), static_cast<Uint8>(V * 255), static_cast<Uint8>(t * 255));
            case 3: return Color(static_cast<Uint8>(p * 255), static_cast<Uint8>(q * 255), static_cast<Uint8>(V * 255));
            case 4: return Color(static_cast<Uint8>(t * 255), static_cast<Uint8>(p * 255), static_cast<Uint8>(V * 255));
            case 5: return Color(static_cast<Uint8>(V * 255), static_cast<Uint8>(p * 255), static_cast<Uint8>(q * 255));
            default: return Color(0, 0, 0); // En cas d'erreur
        }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float linearToGamma(float v) 
    { return v >= 0.0031308f ? 1.055f * powf(v, 1 / 2.4f) - 0.055f : 12.92f * v; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Color OKLABtoRGB(float L, float A, float B) {
        float l = L + A * + 0.3963377774f + B * + 0.2158037573f;
        float m = L + A * - 0.1055613458f + B * - 0.0638541728f;
        float s = L + A * - 0.0894841775f + B * - 1.2914855480f;
        l = powf(l, 3.f); m = powf(m, 3.f); s = powf(s, 3.f);
        float r = l * +4.0767416621 + m * -3.3077115913 + s * +0.2309699292;
        float g = l * -1.2684380046 + m * +2.6097574011 + s * -0.3413193965;
        float b = l * -0.0041960863 + m * -0.7034186147 + s * +1.7076147010;
        r = 255 * linearToGamma(r); g = 255 * linearToGamma(g); b = 255 * linearToGamma(b);
        r = Clip(r, 0, 255); g = Clip(g, 0, 255); b = Clip(b, 0, 255);
        return Color(static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b));
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Color RdmColor(float s, float v)
    { return HSVtoRGB(Rdm(0.f, 360.f), s, v); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void setFloatRect(FloatRect& fr, const Vector2f& pos, const Vector2f& size) {
        fr.left = pos.x - size.x/2; fr.top = pos.y - size.y/2;
        fr.width = size.x; fr.height = size.y;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f AnchorVec(Vector2f& pos1, Vector2f& pos2) {
        float a = Math_Angle(pos1, pos2);
        float d = Math_Dist(pos1, pos2);
        return Vector2f(a, d);
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float CubicOutFunc(float x) { return 1 - powf((1 - x), 3); }
    float CubicInFunc(float x) { return powf(x, 3); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f IntersectTwoCircles(Vector2f posA, Vector2f posB, float r1, float r2, bool returnFisrt) {
    //-------------//
        Vector2f centerD = posA - posB;
        float R = sqrtf((centerD.x * centerD.x) + (centerD.y * centerD.y));
        // No Intersection exist
        if (!(abs(r1 - r2) <= R && R <= r1 + r2)) {
            float a = Math_Angle(posA, posB);
            posA.x += cosf(a) * r1;
            posA.y += sinf(a) * r1;
            return posA;
        }
    //-------------//
        // Intersection exist
        float R2 = R * R;
        float R4 = R2 * R2;
        float a = (r1 * r1 - r2 * r2) / (2 * R2);
        float r2r2 = (r1 * r1 - r2 * r2);
        float c = sqrtf(2 * (r1 * r1 + r2 * r2) / R2 - (r2r2 * r2r2) / R4 - 1);
    //-------------//
        float fx = (posA.x + posB.x) / 2 + a * (posB.x - posA.x);
        float gx = c * (posB.y - posA.y) / 2;
    //-------------//
        float fy = (posA.y + posB.y) / 2 + a * (posB.y - posA.y);
        float gy = c * (posA.x - posB.x) / 2;
    //-------------//
        if (returnFisrt) {
            return Vector2f((fx + gx), (fy + gy));
        } return Vector2f((fx - gx), (fy - gy));
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float ShortestAngleDif(float a1, float a2) {
    //-------------//
        static float a = toRad(540);
        return fmod(capRad(a1 - a2) + a, PI2) - b2_pi;
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    /*float LongestAngleDif(float a1, float a2) {
    //-------------//
        float a = capRad(a1) - capRad(a2);
        float b = a + PI2, c = a - PI2;
    //-------------//
        float abs_a = abs(a);
        float abs_b = abs(b);
        float abs_c = abs(c);
    //-------------//
        float dif = min(min(abs_a, abs_b), abs_c);
        float sign = (dif == abs_a) ? Sign(a) : ((dif == abs_b) ? Sign(b) : Sign(c));
        return dif * sign;
    //-------------//
    }*/
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Catenary::Catenary(int iterations): m_iter(iterations) {}
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Catenary::Solve(Vector2f p1, Vector2f p2, float l) {
    //====================//
        m_isFlip = (p1.x > p2.x);
        m_p1 = p1, m_p2 = p2;
        if ( m_isFlip ) {
            swap( p1.x, p2.x );
            swap( p1.y, p2.y );
        } m_l = l;
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float d = hypot( dx, dy );
    //====================//
        if (l > (d + 0.05f)) {
            if (abs(dx) > 2.f) {
            //===========//
                m_mode = 3;
            //===========//
                const float c = sqrtf( m_l * m_l - dy * dy );
                auto f = [&dx, &c](float a) -> bool {
                    return ( c < 2 * a * sinhf( dx / ( 2 * a ) ) );
                }; const float step = 1.f; float a = 0.1f;
                do { a += step; } while (f(a));
            //===========//
                float a_prev = a - step;
                float a_next = a; int count = 0;
                while ( (a_next - a_prev > EPSILON) && (count < (m_iter)) ) {
                    a = (a_prev + a_next) * 0.5f; count++;
                    if (f(a)) { a_prev = a; } else { a_next = a; }
                } a = -a; m_a = a;
            //===========//
                m_x = ( p1.x + p2.x - a * logf( ( l + dy ) / ( l - dy ) ) ) * 0.5f;
                m_y = ( p1.y + p2.y - l * ( 1 / tanhf( dx / ( 2 * a ) ) ) ) * 0.5f;
                m_c = -a * sinhf( ( p1.x - m_x ) / a );
            //===========//
            } else {
                m_mode = 2;
                m_midp.x = (p1.x + p2.x) * 0.5f;
                m_midp.y = (p1.y + p2.y + l) * 0.5f;
            }
        } else { m_mode = 1; }
    //====================//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f Catenary::getPoint(float t) {
    //-------------//
        assert( (t >= 0.f && t <= 1.f) );
        Vector2f p;
        switch (m_mode) {
        //-------------//
            case 1:
                {Vector2f d = m_p2 - m_p1;
                p.x = m_p1.x + d.x * t;
                p.y = m_p1.y + d.y * t;}
                break;
        //-------------//
            case 2:
                p.y = m_p1.y + m_l * t;
                if (t > ((m_midp.y - m_p1.y) / m_l))
                { p.y = 2 * m_midp.y - p.y; }
                p.x = m_p1.x; break;
        //-------------//
            case 3:
                p.x = getX(t);
                p.y = getY(p.x);
                break;
        //-------------//
        } return p;
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void Catenary::setIterations(int i) { m_iter = i; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Catenary::getY(float x)
    { return m_a * coshf( ( x - m_x ) / m_a ) + m_y; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float Catenary::getX(float t) {
    //-------------//
        t = m_isFlip ? (1 - t) : t;
        return m_a * asinhf( ( t * m_l - m_c ) / m_a ) + m_x;
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    int8_t Catenary::getMode() { return m_mode; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    Vector2f Catenary::getMidP() { return m_midp; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    PIDcontroller::PIDcontroller(float P, float I, float D)
    : m_gainP(P), m_gainI(I), m_gainD(D) {
        m_valueDerivativeMode = true;
        m_dSwitch = false;
        m_limitI = 1.f;
        m_outputMin = -1.f;
        m_outputMax = 1.f;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    float PIDcontroller::update(float c, float t, float dt, bool angleMode) {
    //-------------//
        float error = (angleMode) ? ShortestAngleDif(t, c) : t - c;
        float derivative = 0.f;
        if (m_dSwitch) {
            if (m_valueDerivativeMode) {
                float dif = (angleMode) ? ShortestAngleDif(c, m_lastValue) : (c - m_lastValue);
                derivative = -(dif / dt);
            } else {
                float dif = (angleMode) ? ShortestAngleDif(error, m_lastError) : (error - m_lastError);
                derivative = (dif / dt);
            }
        } else { m_dSwitch = true; }
        m_lastValue = c, m_lastError = error;
        m_storedI = clamp(m_storedI + error * dt, -m_limitI, m_limitI);
    //-------------//
        float P = m_gainP * error;
        float I = m_gainI * m_storedI;
        float D = m_gainD * derivative;
        return clamp(P + I + D, m_outputMin, m_outputMax);
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void PIDcontroller::setPID(float P, float I, float D)
    { m_gainP = P, m_gainI = I, m_gainD = D; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void PIDcontroller::setOutputLimit(float min, float max)
    { m_outputMin = min, m_outputMax = max; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void PIDcontroller::reset() { m_dSwitch = false; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void PIDcontroller::setValueDerivativeMode(bool b)
    { m_valueDerivativeMode = b; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//