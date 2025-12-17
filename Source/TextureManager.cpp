#include "TextureManager.hpp"
#include <iostream>

sf::Texture &TextureManager::load(const std::string &name)
{
  if (!textures_.contains(name))
  {
    if (!textures_[name].loadFromFile("Content\\Textures\\" + name + ".png"))
    {
      std::cerr << "Failed to load player texture from: " << name << '\n';
    }
  }
  return textures_[name];
}