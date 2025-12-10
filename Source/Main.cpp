#include <SFML/Graphics.hpp>

#include "Azure.hpp"

int main()
{
    AzureTower::Game game;

    while (game.IsRunning())
    {
        game.ProcessEvents();
        game.Update();
        game.Render();
    }
}