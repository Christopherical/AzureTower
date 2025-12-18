#pragma once

#include <string>

#include <SFML/Graphics.hpp>

namespace AzureTower
{
class Building
{
public:
  int zone_{};
  std::string name_{};
  sf::Vector2f position_{};
  std::optional<sf::Sprite> sprite_;
  Building(int zone, std::string name, sf::Vector2f position, sf::Texture &texture);
};
} // namespace AzureTower