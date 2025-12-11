#pragma once

#include <SFML/Graphics.hpp>

#include "AzureConfig.hpp"

namespace AzureTower
{

    enum Action
    {
        MoveLeft,
        MoveRight
    };

    struct Player
    {
        int health = PLAYER_HEALTH;
        int speed = PLAYER_SPEED;
        int gold = PLAYER_STARTING_GOLD;
    };

    struct Enemy
    {
        int health;
        int speed;
        float knockBackStrength = KNOCKBACK_STRENGTH;
    };

    struct Stats
    {
        int score;
        int highScore;
        int lives;

        sf::Font font{FONT_PATH};
        sf::Text scoreText{font, "Score: 0", 24};
    };

}