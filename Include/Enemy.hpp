#pragma once

#include <optional>
#include <string>

#include "AzureConfig.hpp"
#include <SFML/Graphics.hpp>

namespace AzureTower
{
class Enemy
{
public:
  std::string name_;
  int health_;
  int speed_;

  std::optional<sf::Sprite> sprite_;
  Enemy(std::string name, int health, int speed, sf::Vector2f position, sf::Texture &texture);
};

} // namespace AzureTower