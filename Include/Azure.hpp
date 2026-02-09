#pragma once

#include <array>
#include <optional>
#include <vector>

#include "Enemy.hpp"
#include "Player.hpp"
#include "TextureManager.hpp"
#include "tilemap.hpp"

namespace AzureTower
{
class Game
{
private:
  // Core Systems.
  sf::RenderWindow window_;
  sf::View camera_;
  sf::Clock clock_;
  sf::Clock enemyClock_;
  TextureManager textureManager_;
  sf::Font font_;
  std::optional<sf::Cursor> cursor_;

  sf::Clock fadeClock_;
  int fadeCount_ = 80;
  uint8_t transparentNumber_;
  bool attackRangeIndicator_ = false;

  // Game Entities.
  Player player_;
  std::vector<Enemy> enemies_;

  // State.
  sf::Vector2f velocity_{0.f, 0.f}; // For smooth sliding/knockback
  float zoomLevel_ = 1.0f;

  // Graphics.
  std::optional<sf::Text> gameOverText_;
  TileMap backgroundTilemap_;

public:
  bool gameOver_ = false;
  Game();

  void InitGame();
  void InitPlayer();

  void ProcessEvents();
  void Update();
  void Render();
  void GameOver();
  void EnemyCollisionCheck(Enemy& enemy);
  sf::Vector2f enemySpawnPosition(sf::FloatRect playerPosition);
  // void Knockback();
  bool IsRunning() const;
  void spawnSlime();
};
} // namespace AzureTower