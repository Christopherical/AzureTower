#include "Azure.hpp"
#include "Enemy.hpp"
#include "tilemap.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>


// TODO - Create new tilemap. Have it so the connecting pixels are the same so they can be mixed up and not look incorrect

// Today//
// Sort out sprites and sizes and understand postion relative to local, global, cameras //
// Cut map up into squares//
// Redo Map and Perspective? //
// Change projectile to projectile image //
// Wc3 Check against //

//  One Tower Pixel art. Use Colour pallete. Main colour, shading, highlight.
//  Shade depending on direction of light.
//  - Dithering - Anti-Aliasing?
// Today//
////////// TODO - Add different Enemy Types. //////////
////////// TODO2 - Add Game Over Screen. //////////
////////// TODO - Add Movement for Enemies. //////////
////////// TODO2 - Add Actual Background. //////////
////////// TODO - Add SPD Logging. //////////
////////// TODO - Add Music. //////////
////////// TODO - Add enemy Pathfinding //////////
// ---NAVIGATION MESH. A*? (Multiple - Crown Simulation,
// flocking, float fields ,RVO (reciprocal veocity avoidance - good for smaller amounts)
// -- what is network prediction/rollback
////////// TODO - Add Diagonal movement adjustment 1.4 //////////

using namespace AzureTower;

AzureTower::Game::Game()
    : window_{sf::VideoMode({GAME_WIDTH, GAME_HEIGHT}), "Azure Tower", sf::State::Fullscreen},
      font_{FONT_PATH}
{
  InitGame();
  InitPlayer();
}

void AzureTower::Game::InitGame()
{
  window_.setKeyRepeatEnabled(false);
  window_.setFramerateLimit(FRAME_RATE);

  sf::Image cursorImage{CURSOR_TEXTURE_PATH};
  cursor_.emplace(cursorImage.getPixelsPtr(), cursorImage.getSize(), sf::Vector2u{0, 0});
  window_.setMouseCursor(cursor_.value());

  camera_.setSize({CAMERA_WIDTH, CAMERA_HEIGHT});

  enemies_.reserve(MAX_ENEMIES);
  towers_.reserve(MAX_TOWERS);
  projectiles_.reserve(MAX_PROJECTILES);

  if (!backgroundTilemap_.load(
          "C:\\Users\\Chris\\Desktop\\rpgPrototype\\AzureTower\\Content\\Textures\\StoneTileMap2.png", {32, 32},
          LEVEL_ONE.data(), 24, 13))
  {
    std::cerr << "Failed to load tilemap!" << std::endl;
  }
  backgroundTilemap_.setScale({5.f, 5.f});
}

void AzureTower::Game::InitPlayer()
{
  player_.sprite_.emplace(textureManager_.load(PLAYER_NAME));
  player_.sprite_->setScale(SPRITE_SCALE);
  player_.sprite_->setOrigin(player_.sprite_->getLocalBounds().size / 2.f);

  // // Position at background center
  // auto bgBounds = backgroundSprite_->getGlobalBounds();
  // player_.sprite_->setPosition(bgBounds.position + bgBounds.size / 2.f);

  camera_.setCenter(player_.sprite_->getGlobalBounds().position);
}

void Game::ProcessEvents()
{
  while (const std::optional event = window_.pollEvent())
  {
    if (event->is<sf::Event::Closed>())
    {
      window_.close();
    }
    else if (auto *mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
    {
      switch (mouseEvent->button)
      {
        // case sf::Mouse::Button::Left:
        //   auto mouseWorldPos = window_.mapPixelToCoords(sf::Mouse::getPosition(window_), camera_);

        //   // Testing creation of buildings on mouse click.
        //   towers_.emplace_back(1, "house", mouseWorldPos, textureManager_.load("house"));

        //   // Testing clicking on enemies.
        //   for (auto &enemy : enemies_)
        //   {
        //     if (enemy.sprite_->getGlobalBounds().contains(mouseWorldPos))
        //     {
        //       enemy.sprite_->setColor(sf::Color::Blue);
        //       break;
        //     }
        //   }
      }
    }
    else if (auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
    {
      // Handle one-time key presses here (menus, actions, etc.)
      switch (keyEvent->code)
      {
        case sf::Keyboard::Key::Escape: window_.close(); break;
        case sf::Keyboard::Key::F: // Toggle Attack Range Indicators.
          attackRangeIndicator_ = (attackRangeIndicator_) ? false : true;
        default: break;
      }
    }
    // else if (auto *scrollEvent = event->getIf<sf::Event::MouseWheelScrolled>())
    // {
    //   if (scrollEvent->wheel == sf::Mouse::Wheel::Vertical)
    //   {
    //     float zoomFactor = (scrollEvent->delta > 0) ? 0.9f : 1.1f;
    //     float newZoom = zoomLevel_ * zoomFactor;

    //     if (newZoom >= MIN_CAMERA_ZOOM && newZoom <= MAX_CAMERA_ZOOM)
    //     {
    //       zoomLevel_ = newZoom;
    //       camera_.zoom(zoomFactor);
    //     }
    //   }
    // }
  }
}

void AzureTower::Game::Update()
{
  if (gameOver_)
    return;

  float deltaTime = clock_.restart().asSeconds();
  spawnSlime();

  // Player input & movement.
  sf::Vector2f movement{0.f, 0.f};
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    movement.y -= player_.speed_ * deltaTime;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    movement.y += player_.speed_ * deltaTime;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    movement.x -= player_.speed_ * deltaTime;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    movement.x += player_.speed_ * deltaTime;
  player_.sprite_->move(movement);

  // Movement + Collision checks.
  float slimeMovement = SLIME_SPEED * deltaTime; // TODO - Currently Hardcoded
  for (auto &enemy : enemies_)
  {
    enemy.sprite_->move({0.f, slimeMovement});
    EnemyCollisionCheck(enemy);
  }
  for (auto &projectile : projectiles_)
  {
    projectile.projectileSprite_->move(projectile.direction_ * PROJECTILE_SPEED);
    ProjectileCollisionCheck(projectile);
  }
  for (auto &tower : towers_)
  {
    TowerCollisionCheck(tower); // TODO - Potentially change to: (Inverting the loop)
    //       for (auto &enemy : enemies_) {
    //   for (auto &tower : towers_) {
    //     if (towerCanHitEnemy(tower, enemy)) { ... break; }
    //   }
    // }
  }

  // Testing Fading - TODO - Make a class/Func
  if (fadeClock_.getElapsedTime().asSeconds() > 0.1)
  {
    fadeClock_.restart();
    fadeCount_ += 15;
    int cycle = fadeCount_ % 512;
    int rawCycle = (cycle < 256) ? cycle : 511 - cycle;
    transparentNumber_ = 80 + (rawCycle * 120 / 255);
  }

  // End of Frame updates - Removal, Camera set, Game over check.
  enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [](const Enemy &enemy) { return enemy.isDead_; }),
                 enemies_.end());
  projectiles_.erase(std::remove_if(projectiles_.begin(), projectiles_.end(),
                                    [](const Projectile &projectile) { return projectile.isDead_; }),
                     projectiles_.end());
  
  // Round camera position to whole pixels to prevent tile rippling
  sf::Vector2f playerPos = player_.sprite_->getGlobalBounds().position;
  camera_.setCenter({std::round(playerPos.x), std::round(playerPos.y)});
  
  GameOver();
}

void AzureTower::Game::EnemyCollisionCheck(Enemy &enemy)
{
  if (enemy.sprite_->getPosition().y > BOTTOM_THRESHOLD)
  {
    player_.health_ = player_.health_ - 50; // TODO - Add Health
    enemy.isDead_ = true;
  }
}

void AzureTower::Game::ProjectileCollisionCheck(Projectile &projectile)
{
  if (projectile.isDead_)
    return;

  auto projectileBounds = projectile.projectileSprite_->getGlobalBounds();
  for (auto &enemy : enemies_)
  {
    if (projectileBounds.findIntersection(enemy.sprite_->getGlobalBounds()))
    {
      // TODO - Play Hit Sound.
      enemy.isDead_ = true;
      projectile.isDead_ = true;
      break;
    }
  }
  // TODO - Change from hardcoded.
  if (projectile.projectileSprite_->getPosition().y > BOTTOM_THRESHOLD ||
      projectile.projectileSprite_->getPosition().x < 0 || projectile.projectileSprite_->getPosition().y < 0 ||
      projectile.projectileSprite_->getPosition().x > 1195)
  {
    projectile.isDead_ = true;
  }
}

void AzureTower::Game::TowerCollisionCheck(Tower &tower)
{
  if (tower.projectileClock_.getElapsedTime().asSeconds() <= 0.5f)
    return;

  sf::Vector2f towerPos = tower.sprite_->getPosition();
  tower.projectileClock_.restart();
  for (auto &enemy : enemies_)
  {
    // Check if enemy is within circular attack range
    sf::Vector2f enemyPos = enemy.sprite_->getPosition();
    float dx = enemyPos.x - towerPos.x;
    float dy = enemyPos.y - towerPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance <= tower.attackRange_.getRadius())
    {
      if (distance > 0.001f)
        tower.projectileDirection_ = sf::Vector2f(dx, dy) / distance;
      projectiles_.emplace_back(tower.projectileDirection_, towerPos, textureManager_.load("MediumYellowBall"));
      break;
    }
  }
}

void AzureTower::Game::spawnSlime()
{
  if (enemyClock_.getElapsedTime().asSeconds() > 2.0f && enemies_.size() < MAX_ENEMIES)
  {
    enemies_.emplace_back(SLIME_NAME, SLIME_HEALTH, SLIME_SPEED, SLIME_POSITION, textureManager_.load(SLIME_NAME));
    enemyClock_.restart();
  }
}

void AzureTower::Game::GameOver()
{
  if (player_.health_ <= 0) // TODO Add Health
  {
    gameOverText_.emplace(font_);
    gameOverText_->setString("GAME OVER");
    gameOverText_->setScale({2.f, 2.f});
    gameOverText_->setFillColor(sf::Color::Red);
    gameOverText_->setOrigin(gameOverText_->getLocalBounds().size / 2.f);
    gameOver_ = true;
  }
}

void AzureTower::Game::Render()
{
  window_.clear();
  window_.setView(camera_);

  // Draw tilemap background first
  window_.draw(backgroundTilemap_);

  // Handle game over screen
  if (gameOver_ && gameOverText_)
  {
    // gameOverText_->setPosition(camera_.getCenter()); 
    window_.draw(*gameOverText_);
    window_.display();
    return;
  }

  // Normal game rendering
  window_.draw(*player_.sprite_);

  for (auto &enemy : enemies_)
  {
    if (enemy.sprite_)
    {
      window_.draw(*enemy.sprite_);
    }
  }
  for (auto &tower : towers_)
  {
    if (tower.sprite_)
    {
      window_.draw(*tower.sprite_);
      if (attackRangeIndicator_)
      {
        window_.draw(tower.attackRange_);
      }
    }
  }
  for (auto &projectile : projectiles_)
  {
    window_.draw(*projectile.projectileSprite_);
  }
  // window_.draw(*backgroundSprite_);
  window_.display();
}

bool AzureTower::Game::IsRunning() const
{
  return window_.isOpen();
}
