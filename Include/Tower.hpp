#pragma once

#include <string>

#include <SFML/Graphics.hpp>

namespace AzureTower
{
class Tower
{
public:
  int zone_{}; // TODO
  int kills{}; // TODO
  int level{}; // TODO
  sf::Clock projectileClock_;
  sf::Vector2f projectileDirection_;
  bool projectileDead_;
  bool AttackRangeOn_{false};

  std::string name_{};
  sf::Vector2f position_{};
  sf::CircleShape attackRange_{};
  std::optional<sf::Sprite> sprite_;
  Tower(int zone, std::string name, sf::Vector2f position, sf::Texture &texture);
};

struct Projectile
{
  std::optional<sf::Sprite> projectileSprite_;
  sf::Vector2f direction_;
  bool isDead_ = false;
  Projectile(sf::Vector2f direction, sf::Vector2f TowerPos, sf::Texture &texture)
      : direction_(direction)
  {
    projectileSprite_.emplace(texture);
    projectileSprite_->setOrigin(projectileSprite_->getLocalBounds().size / 2.f);
    projectileSprite_->setPosition(TowerPos);

  }
};
} // namespace AzureTower
