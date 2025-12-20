#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

#include "AzureConfig.hpp"

namespace AzureTower
{
class Player
{
public:
  int health_{PLAYER_HEALTH};
  int speed_{PLAYER_SPEED};

  std::optional<sf::Sprite> sprite_;

};
} // namespace AzureTower