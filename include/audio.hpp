//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #pragma once
    #include <SFML/Audio.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    extern float SoundVolume;
    extern float MusicVolume;
//~~~~~~~~~~~~~~~~~~~~~~~~//
    void VecSound_Play(sf::SoundBuffer* bf, float v = 50.f, float p = 1.f);
    void VecSound_Update(); void VecSound_SuprAll();
    void VevSound_SetMaxLimit(int limit);
//~~~~~~~~~~~~~~~~~~~~~~~~//
    /* TODO
        - Musiques
        - Delay / Reverb
    */
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//