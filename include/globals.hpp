//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    #pragma once
    #include <SFML/Graphics.hpp>
    #include <box2d/box2d.h>
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    const sf::String GameTitle = "Ultra Sandbox";
    const float B2SCALE = 64.f;
    constexpr float PI2 = b2_pi * 2;
    constexpr float RAD90 = b2_pi / 2.f;
    constexpr float RAD45 = b2_pi / 4.f;
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    extern uint16 WIN_W, WIN_H;
    extern sf::Vector2f WIN_DIMS;
    extern sf::Vector2f WIN_CENTER;
    extern sf::Vector2f ZeroVec2f;
    void SetWinDims(sf::Vector2f dims);
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//