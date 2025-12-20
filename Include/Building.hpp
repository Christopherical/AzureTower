#pragma once

#include <string>

#include <SFML/Graphics.hpp>

namespace AzureTower
{
class Building
{
public:
  int zone_{}; // TODO
  int kills{}; // TODO
  int level{}; // TODO
  sf::Clock projectileClock_;
  sf::Vector2f projectileDirection_;
  bool projectileDead_;

  std::string name_{};
  sf::Vector2f position_{};
  sf::CircleShape attackRange_{};
  std::optional<sf::Sprite> sprite_;
  sf::RectangleShape projectile_;
  Building(int zone, std::string name, sf::Vector2f position, sf::Texture &texture);

  void buildingShoot(sf::Vector2f direction);
};
} // namespace AzureTower