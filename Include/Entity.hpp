#pragma once
#include <optional>
#include <SFML/Graphics.hpp>
#include "AzureConfig.hpp"

namespace AzureTower
{
    class Player
    {
    public:
        Player();
        int health_;
        int speed_;
        sf::Vector2f position_;
        std::optional<sf::Sprite> sprite_;

        void InitPlayer();
    };

    class Enemy
    {
    public:
        std::string name_;
        int health_;
        int speed_;
        sf::Vector2f position_;
        std::optional<sf::Sprite> sprite_;
        Enemy(std::string name, int health, int speed, sf::Vector2f position, sf::Texture& texture);
    };

}