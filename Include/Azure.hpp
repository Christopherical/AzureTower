#pragma once

#include "AzureTypes.hpp"

namespace AzureTower
{
    class Game
    {
    private:
        sf::RenderWindow window_;
        sf::Texture texture_;
        sf::Sprite sprite_;
        Player player_;

    public:
        Game();
        void run();
        sf::Sprite getSprite() const;

        void processEvents();
        void Update();
        void Render();

        bool IsRunning() const;
    private:
    };
}