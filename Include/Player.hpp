#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

namespace AzureTower
{
class Player
{
public:
  Player();
  int health_;
  int speed_;
  sf::Vector2f position_;
  std::optional<sf::Sprite> sprite_;

  void InitPlayer();
  void CollidesWith(); // TODO
};
} // namespace AzureTower