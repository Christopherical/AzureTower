#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
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

        sf::Texture texture;
        std::optional<sf::Sprite> sprite;
        Player()
        {
            if (!texture.loadFromFile(PLAYER_TEXTURE_PATH2))
            {
                std::cerr << "Failed to load player texture from: " << PLAYER_TEXTURE_PATH2 << '\n';
            }
            sprite.emplace(texture);
        };
    };

    struct EnemyTemplate
    {
        std::string enemyTemplateName{};
        int enemyHealth{};
        int enemySpeed{};
        float enemyKnockbackStrength{};
        sf::Vector2f enemyPos{};
    };

    const EnemyTemplate SLIME_TEMPLATE = {"slimeRo", 100, 100, 1000.f, {200.f, 200.f}};
    const EnemyTemplate DRAGON_TEMPLATE = {"dragon", 500, 200, 2000.f, {400.f, 400.f}};

    struct Construct
    {
        const std::string name;
        const bool hasCollision;
    };
    struct Enemy
    {
        std::string name{};
        int health{};
        int speed{};
        float knockBackStrength{};
        sf::Vector2f position{};

        sf::Texture texture;
        std::optional<sf::Sprite> sprite;

        Enemy(const EnemyTemplate &enemyTemplate)
            : name(enemyTemplate.enemyTemplateName),
              health(enemyTemplate.enemyHealth),
              speed(enemyTemplate.enemySpeed),
              knockBackStrength(enemyTemplate.enemyKnockbackStrength),
              position(enemyTemplate.enemyPos)
        {
            std::string fullPath = TEXTURE_PATH + name + ".png";
            std::cout << "Attempting to load texture: " << fullPath << '\n';

            if (!texture.loadFromFile(fullPath))
            {
                std::cerr << "Failed to load texture for: " << name << " from path: " << fullPath << '\n';
                return;
            }

            sprite.emplace(texture);
            sprite->setPosition({position.x, position.y});
            sprite->setScale(SPRITE_SCALE);
        }

        // Move constructor - rebind sprite to new texture location
        Enemy(Enemy &&other) noexcept
            : name(std::move(other.name)),
              health(other.health),
              speed(other.speed),
              knockBackStrength(other.knockBackStrength),
              position(other.position),
              texture(std::move(other.texture))
        {
            // Recreate sprite with the moved texture
            if (other.sprite)
            {
                sprite.emplace(texture); // Bind to NEW texture location
                sprite->setPosition(other.sprite->getPosition());
                sprite->setScale(other.sprite->getScale());
            }
        }

        // Delete copy constructor (sprites can't be safely copied)
        Enemy(const Enemy &) = delete;
        Enemy &operator=(const Enemy &) = delete;
        Enemy &operator=(Enemy &&) = delete;
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