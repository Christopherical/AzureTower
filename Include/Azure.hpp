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
  // Core Systems.
  sf::RenderWindow window_;
  sf::View camera_;
  sf::Clock clock_;
  TextureManager textureManager_;

  // Game Entities.
  Player player_;
  std::vector<Enemy> enemies_;
  std::vector<Building> buildings_;
  std::vector<Interactable> interactables_;

  // State.
  sf::Vector2f velocity_{0.f, 0.f}; // For smooth sliding/knockback  
  float zoomLevel_ = 1.0f;

  // Graphics.
  std::optional<sf::Sprite> backgroundSprite_;

public:
  Game();
  
  void InitGame();
  void InitPlayer();

  void ProcessEvents();
  void Update();
  void Render();

  void ZoneLoader(); // TODO ...
  // void Knockback();
  bool IsRunning() const;
  bool CollisionCheck();
};
} // namespace AzureTower