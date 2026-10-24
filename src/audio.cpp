//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #include <audio.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
    using namespace sf;
    using namespace std;
//~~~~~~~~~~~~~~~~~~~~~~~~//
    std::vector<Sound*> LstSound;
    float SoundVolume = 100.f;
    float MusicVolume = 100.f;
    int SoundLimit = 300;
//~~~~~~~~~~~~~~~~~~~~~~~~//
    auto SoundCheck = [](Sound* s) {
        if (s->getStatus() == SoundSource::Stopped) {
            delete s; return true;
        } return false;
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void VevSound_SetMaxLimit(int limit) { SoundLimit = limit; }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void VecSound_Play(SoundBuffer* bf, float v, float p) {
        if (LstSound.size() < SoundLimit && v > 0.5f) {
            Sound* snd = new Sound(*bf);
            if (p < 0.1f) { p = 0.1f; }
            snd->setPitch(p);
            snd->setVolume(v); snd->play();
            LstSound.push_back(snd);
        }
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void VecSound_Update()
    { LstSound.erase(remove_if(LstSound.begin(), LstSound.end(), SoundCheck), LstSound.end()); }
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void VecSound_SuprAll() {
        for (auto& s : LstSound)
        { s->stop(); delete s; }
        LstSound.clear();
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//