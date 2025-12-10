#pragma once
#

#include "AzureTypes.hpp"
#include <optional>

namespace AzureTower
{
    class Game
    {
    private:
        sf::RenderWindow window_;
        sf::Texture texture_;
        std::optional<sf::Sprite> sprite_;
        Player player_;
        sf::RectangleShape testBlock_;

        sf::Clock clock_;
        sf::Vector2f velocity_{0.f, 0.f}; // For smooth sliding/knockback

    public:
        Game();
        void run();
        sf::Sprite getSprite() const;

        void ProcessEvents();
        void Update();
        void Render();

        bool IsRunning() const;
        bool CollisionCheck();
    private:
    };
}