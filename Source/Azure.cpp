#include "Azure.hpp"
#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>

////////// TODO - Add different Enemy Types. //////////
////////// TODO - Add Cooldown for taking damage. //////////
////////// TODO - Add Weapon Swing into Enemy. //////////
////////// TODO - Add Game Over Screen. //////////
////////// TODO - Add Movement for Enemies. //////////
////////// TODO - Add Actual Background. //////////
////////// TODO - Add SPD Logging. //////////
////////// TODO - Add Music. //////////
////////// TODO - Add performance testing with lots of enemies. //////////
////////// TODO - Add nemy Pathfinding //////////
////////// TODO - Add Diagonal movement adjustment 1.4 //////////

using namespace AzureTower;

AzureTower::Game::Game()
    : window_{sf::VideoMode({GAME_WIDTH, GAME_HEIGHT}), "Azure Tower"}
{
  InitGame();
  InitPlayer();
}

void AzureTower::Game::InitGame()
{
  window_.setKeyRepeatEnabled(false);
  window_.setFramerateLimit(FRAME_RATE);
  camera_.setSize({CAMERA_WIDTH, CAMERA_HEIGHT});

  backgroundSprite_.emplace(textureManager_.load(BACKGROUND_NAME));
  backgroundSprite_->setPosition({0.f, 0.f});
  backgroundSprite_->setScale({5.f, 5.f});

  enemies_.reserve(MAX_ENEMIES);
}

void AzureTower::Game::InitPlayer()
{
  player_.sprite_.emplace(textureManager_.load(PLAYER_NAME));
  player_.sprite_->setScale(SPRITE_SCALE);
  player_.sprite_->setOrigin(player_.sprite_->getLocalBounds().size / 2.f);

  // Position at background center
  auto bgBounds = backgroundSprite_->getGlobalBounds();
  player_.sprite_->setPosition(bgBounds.position + bgBounds.size / 2.f);

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
        case sf::Mouse::Button::Left:
          auto mouseWorldPos = window_.mapPixelToCoords(sf::Mouse::getPosition(window_), camera_);

          // Testing creation of buildings on mouse click.
          buildings_.emplace_back(1, "house", mouseWorldPos, textureManager_.load("house"));

          // Testing clicking on enemies.
          for (auto &enemy : enemies_)
          {
            if (enemy.sprite_->getGlobalBounds().contains(mouseWorldPos))
            {
              enemy.sprite_->setColor(sf::Color::Blue);
              break;
            }
          }
      }
    }
    else if (auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
    {
      // Handle one-time key presses here (menus, actions, etc.)
      switch (keyEvent->code)
      {
        case sf::Keyboard::Key::Escape: std::cout << "Escape pressed\n"; break;
        case sf::Keyboard::Key::Q:
          if (player_.sprite_)
            std::cout << player_.sprite_->getPosition().x << " || " << player_.sprite_->getPosition().y << std::endl;
          break;
        default: break;
      }
    }
    else if (auto *scrollEvent = event->getIf<sf::Event::MouseWheelScrolled>())
    {
      if (scrollEvent->wheel == sf::Mouse::Wheel::Vertical)
      {
        float zoomFactor = (scrollEvent->delta > 0) ? 0.9f : 1.1f;
        float newZoom = zoomLevel_ * zoomFactor;

        if (newZoom >= MIN_CAMERA_ZOOM && newZoom <= MAX_CAMERA_ZOOM)
        {
          zoomLevel_ = newZoom;
          camera_.zoom(zoomFactor);
        }
      }
    }
  }
}

void AzureTower::Game::Update()
{
  float deltaTime = clock_.restart().asSeconds();
  if (enemyClock_.getElapsedTime().asSeconds() > 1.0f && enemies_.size() < MAX_ENEMIES)
  {
    enemies_.emplace_back(SLIME_NAME, SLIME_HEALTH, SLIME_SPEED, SLIME_POSITION, textureManager_.load(SLIME_NAME));
    enemyClock_.restart();
  }

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
  // TODO ...
  /*
  - if enemy in range
  - create projectile
  - shoot projectile in direction of enemy
  - each update relative to deltatime the projectile moves towards enemy
  - check for projectile x enemy collision
  - if it hits delete projectile
  - lower enemy health
  - some interaction that they've been hit?
  */
  // Checking each building against each enemy // TODO Function to work out which enemy is closest.
  for (auto &building : buildings_)
  {
    for (auto &enemy : enemies_)
    {
      // TODO - Make this check circular
      if (building.attackRange_.getGlobalBounds().findIntersection(enemy.sprite_->getGlobalBounds()))
      {
        building.buildingShoot(enemy.sprite_->getPosition());
        auto thingg = (building.sprite_->getPosition() - enemy.sprite_->getPosition());
        building.projectileDirection_ = thingg;
        float length =
            std::sqrt(building.projectileDirection_.x * building.projectileDirection_.x + building.projectileDirection_.y * building.projectileDirection_.y);
        if (length > 0.001f)
        {
          building.projectileDirection_ /= length; // TODO Understand this.
        }
      }
      if(building.projectile_.getGlobalBounds().findIntersection(enemy.sprite_->getGlobalBounds())){
        enemy.isDead_ = true;
        building.projectile_ = sf::RectangleShape{};
      }
    }
    building.projectile_.move({-(building.projectileDirection_.x + 50), -(building.projectileDirection_.y + 50)});

    // building.projectile_.move({0.f, -(300 * deltaTime)});
  }
  // Enemy movement + boundary check.
  for (auto &enemy : enemies_)
  {
    enemy.sprite_->move(enemy.Movement2(deltaTime));
    if (enemy.sprite_->getPosition().y > BOTTOM_THRESHOLD)
    {
      --player_.health_;
      enemy.isDead_ = true;
    }
  }

  // Enemy Removal.
  enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [](const Enemy &enemy) { return enemy.isDead_; }),
                 enemies_.end());

  // Position Camera on Player after movement.
  camera_.setCenter(player_.sprite_->getGlobalBounds().position);

  // Game over updates.
  if (player_.health_ <= 0)
  {
    backgroundSprite_->setColor(sf::Color::Red);
  }
}

void AzureTower::Game::Render()
{
  window_.clear();
  window_.setView(camera_);
  window_.draw(*backgroundSprite_);
  window_.draw(*player_.sprite_);

  for (auto &enemy : enemies_)
  {
    if (enemy.sprite_)
    {
      window_.draw(*enemy.sprite_);
    }
  }
  for (auto &building : buildings_)
  {
    if (building.sprite_)
    {
      window_.draw(building.attackRange_);
      window_.draw(*building.sprite_);
      window_.draw(building.projectile_);
    }
  }

  window_.display();
}

bool Game::IsRunning() const
{
  return window_.isOpen();
}

bool AzureTower::Game::CollisionCheck()
{
  return false;
}
// Add functions for potentially needed classes
// void Game::Knockback()
// {
//   // sf::Vector2i mousePos = sf::Mouse::getPosition(window_);

//   sf::Time elapsed = clock_.getElapsedTime();
//   float deltaTime = clock_.restart().asSeconds();

//   sf::Vector2f movement{0.f, 0.f};

//   // Check which axes have significant knockback velocity
//   float velocityThreshold = 50.0f;
//   bool knockbackX = std::abs(velocity_.x) > velocityThreshold;
//   bool knockbackY = std::abs(velocity_.y) > velocityThreshold;

//   // Gather input - only block movement on axes with active knockback
//   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && !knockbackY)
//     movement.y -= player_.speed * deltaTime;
//   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && !knockbackY)
//     movement.y += player_.speed * deltaTime;
//   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !knockbackX)
//     movement.x -= player_.speed * deltaTime;
//   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && !knockbackX)
//     movement.x += player_.speed * deltaTime;

//   // Apply velocity (sliding/knockback)
//   player_.sprite->move(velocity_ * deltaTime);

//   // Apply friction to slow down velocity over time
//   float friction = 5.0f; // Higher = stops faster
//   velocity_.x *= (1.0f - friction * deltaTime);
//   velocity_.y *= (1.0f - friction * deltaTime);

//   // Stop velocity if it's very small
//   if (std::abs(velocity_.x) < 1.0f)
//     velocity_.x = 0.f;
//   if (std::abs(velocity_.y) < 1.0f)
//     velocity_.y = 0.f;

//   // Apply player input movement
//   player_.sprite->move(movement);

//   sf::Vector2f originalPos = player_.sprite->getPosition();

//   // Check collision after movement
//   auto playerBox = player_.sprite->getGlobalBounds();
//   auto enemyBox = slime_.sprite->getGlobalBounds();

//   if (playerBox.findIntersection(enemyBox))
//   {
//     // Collision detected - revert position
//     player_.sprite->setPosition(originalPos);

//     // Calculate knockback direction using vector subtraction
//     sf::Vector2f playerPos = player_.sprite->getPosition();
//     sf::Vector2f enemyPos = slime_.sprite->getPosition();
//     sf::Vector2f knockbackDirection = playerPos - enemyPos; // Vector from enemy to player

//     // Normalize the direction vector
//     float length = std::sqrt(knockbackDirection.x * knockbackDirection.x + knockbackDirection.y *
//     knockbackDirection.y); if (length > 0.001f)
//     {
//       knockbackDirection /= length; // Make it a unit vector

//       // TODO make velocity and knockback strength specific to entity.
//       // Apply knockback velocity
//       velocity_ = knockbackDirection * slime_.knockBackStrength;
//     }
//   }
// }
