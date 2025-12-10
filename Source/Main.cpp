#include <SFML/Graphics.hpp>

#include "Azure.hpp"

int main()
{
    AzureTower::Game game;

    while (game.IsRunning())
    {
        game.processEvents();
        // game.Update();
        game.Render();
    }
}