#include "Azure.hpp"
#include "Enemy.hpp"
#include "tilemap.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

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

  // sf::Image cursorImage{CURSOR_TEXTURE_PATH};
  // cursor_.emplace(cursorImage.getPixelsPtr(), cursorImage.getSize(), sf::Vector2u{0, 0});
  // window_.setMouseCursor(cursor_.value());
  window_.setMouseCursorVisible(false);

  camera_.setSize({CAMERA_WIDTH, CAMERA_HEIGHT});

  enemies_.reserve(MAX_ENEMIES);

  // if (!backgroundTilemap_.load(
  //         "C:\\Users\\Chris\\Desktop\\rpgPrototype\\AzureTower\\Content\\Textures\\StoneTileMap2.png", {32, 32},
  //         LEVEL_ONE.data(), 24, 13))
  // {
  //   std::cerr << "Failed to load tilemap!" << std::endl;
  // }
  // backgroundTilemap_.setScale({5.f, 5.f});
}

void AzureTower::Game::InitPlayer()
{
  player_.sprite_.emplace(textureManager_.load(PLAYER_NAME));
  // player_.sprite_->setScale(SPRITE_SCALE);
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
        // case sf::Keyboard::Key::F: // Toggle Attack Range Indicators.
        //   attackRangeIndicator_ = (attackRangeIndicator_) ? false : true;
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
  auto playerPosition = player_.sprite_->getGlobalBounds();

  for (auto &enemy : enemies_)
  {
    enemy.sprite_->move({0.f, slimeMovement});
    EnemyCollisionCheck(enemy);
  }
 
  // End of Frame updates - Removal, Camera set, Game over check.
  enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [](const Enemy &enemy) { return enemy.isDead_; }),
                 enemies_.end());
  
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

sf::Vector2f AzureTower::Game::enemySpawnPosition(sf::FloatRect playerPosition){
  // Take player position.
  // Return positions that enemies can spawn. A circle around the player. Monsters can spawn on its perimeter.
  sf::Vector2f enemySpawnLocation({playerPosition.position.x + 100, playerPosition.position.y + 100}); // Hardcoded. Randomize this.

  // Check if other enemies are already on that position.
  // Check if monster cap has been reached.
  // Return random location on that circle's boundary.
  return enemySpawnLocation;
}

void AzureTower::Game::spawnSlime()
{
  auto enemySpawnLocation = enemySpawnPosition(player_.sprite_->getGlobalBounds());

  if (enemyClock_.getElapsedTime().asSeconds() > 2.0f && enemies_.size() < MAX_ENEMIES)
  {
    enemies_.emplace_back(SLIME_NAME, SLIME_HEALTH, SLIME_SPEED, enemySpawnLocation, textureManager_.load(SLIME_NAME));
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
  // window_.draw(backgroundTilemap_);

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
  // window_.draw(*backgroundSprite_);
  window_.display();
}

bool AzureTower::Game::IsRunning() const
{
  return window_.isOpen();
}
