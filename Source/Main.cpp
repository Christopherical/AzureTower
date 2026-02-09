#include <SFML/Graphics.hpp>
#include <iostream>
#include "Azure.hpp"

// Tell NVIDIA and AMD drivers to use dedicated GPU
extern "C"
{
  __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

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