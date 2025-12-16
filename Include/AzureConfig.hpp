#pragma once

#include <string>

namespace AzureTower
{
    constexpr int GAME_WIDTH = 800;
    constexpr int GAME_HEIGHT = 600;

    constexpr int PLAYER_HEALTH = 100;
    constexpr int PLAYER_SPEED = 300;
    constexpr int PLAYER_STARTING_GOLD = 500;

    constexpr int SLIME_HEALTH = 50;
    constexpr int SLIME_SPEED = 2;
    constexpr float KNOCKBACK_STRENGTH = 1000.f;

    const sf::Vector2f SPRITE_SCALE = {3.f, 3.f};

    const std::string FONT_PATH = "Content\\Fonts\\arial.ttf";
    const std::string TEXTURE_PATH = "Content\\Textures\\";
    const std::string PLAYER_TEXTURE_PATH = "Content\\Textures\\player.png";
    const std::string SLIME_TEXTURE_PATH = "Content\\Textures\\slimeRo.png";
}