#pragma once

#include <optional>
#include <vector>

#include "Building.hpp"
#include "Enemy.hpp"
#include "Interactable.hpp"
#include "Player.hpp"
#include "TextureManager.hpp"

namespace AzureTower
{
class Game
{
private:
  sf::RenderWindow window_;
  sf::Clock clock_;
  TextureManager textureManager_;
  Player player_;
  std::vector<Enemy> enemies_;
  std::vector<Building> buildings_;
  std::vector<Interactable> interactables_;
  sf::Vector2f velocity_{0.f, 0.f}; // For smooth sliding/knockback

  std::optional<sf::Sprite> houseSprite_;
  std::optional<sf::Sprite> backgroundSprite_;

public:
  Game();

  void ProcessEvents();
  void Update();
  void Render();

  void ZoneLoader();
  // void Knockback();
  bool IsRunning() const;
  bool CollisionCheck();

private:
  // void InitPlayer();
};
} // namespace AzureTower