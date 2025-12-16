#pragma once

#include "AzureTypes.hpp"
#include <vector>
#include <optional>

namespace AzureTower
{
    class Game
    {
    private:
        sf::RenderWindow window_;
        sf::Clock clock_;

        Player player_;
        std::vector<Enemy> enemies_;

        sf::Vector2f velocity_{0.f, 0.f}; // For smooth sliding/knockback

    public:
        Game();

        void ProcessEvents();
        void Update();
        void Render();

        // void Knockback();
        bool IsRunning() const;
        bool CollisionCheck();

    private:
        void InitPlayer();
        // void InitSlime();
    };
}