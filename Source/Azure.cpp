#include "Azure.hpp"
#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>

////////// TODO - Add different Enemy Types. //////////
////////// TODO - Add Cooldown for taking damage. //////////
////////// TODO - Add Knockback? //////////
////////// TODO - Weapon Swing into Enemy. //////////
////////// TODO - Add Game Over Screen. //////////
////////// TODO - Add Movement for Enemies. //////////
////////// TODO - Add Actual Background. //////////
////////// TODO - Add SPD Logging. //////////
////////// TODO - Add Music. //////////
////////// TODO - Add performance testing with lots of enemies. //////////

using namespace AzureTower;

AzureTower::Game::Game()
    : window_{sf::VideoMode({GAME_WIDTH, GAME_HEIGHT}), "Azure Tower"}
{
  InitGame();
  InitPlayer();

  enemies_.emplace_back(SLIME_NAME, SLIME_HEALTH, SLIME_SPEED, SLIME_POSITION, textureManager_.load(SLIME_NAME));
  buildings_.emplace_back(1, BUILDING_NAME, BUILDING_POSITION, textureManager_.load(BUILDING_NAME));
}

void AzureTower::Game::InitGame()
{
  window_.setKeyRepeatEnabled(false);
  window_.setFramerateLimit(FRAME_RATE);
  camera_.setSize({CAMERA_WIDTH, CAMERA_HEIGHT});

  backgroundSprite_.emplace(textureManager_.load(BACKGROUND_NAME));
  backgroundSprite_->setPosition({0.f, 0.f});
  backgroundSprite_->setScale(SPRITE_SCALE);
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
  sf::Vector2i mousePos = sf::Mouse::getPosition(window_);

  sf::Time elapsed = clock_.getElapsedTime();
  float deltaTime = clock_.restart().asSeconds();

  sf::Vector2f movement{0.f, 0.f};
  sf::Vector2f originalPos = player_.sprite_->getPosition();

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    movement.y -= player_.speed_ * deltaTime;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    movement.y += player_.speed_ * deltaTime;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    movement.x -= player_.speed_ * deltaTime;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    movement.x += player_.speed_ * deltaTime;

  player_.sprite_->move(movement);

  // Check collision after movement
  auto playerBox = player_.sprite_->getGlobalBounds();
  for (auto &enemy : enemies_)
  {
    auto enemyBox = enemy.sprite_->getGlobalBounds();
    if (playerBox.findIntersection(enemyBox))
    {
      player_.health_ -= 10;
      player_.sprite_->setPosition(originalPos);
    }
  }

  for (auto &building : buildings_)
  {
    auto buildingBox = building.sprite_->getGlobalBounds();

    // Remove roof from collision
    float roofHeight = 60.f;
    buildingBox.position.y += roofHeight;
    buildingBox.size.y -= roofHeight;

    if (playerBox.findIntersection(buildingBox))
    {
      player_.sprite_->setPosition(originalPos);
    }
  }

  camera_.setCenter(player_.sprite_->getGlobalBounds().position);
  if(player_.health_ <=0){
    backgroundSprite_->setColor(sf::Color::Red);
  }
}

void AzureTower::Game::ZoneLoader()
{
  // .. TODO
}

void AzureTower::Game::Render()
{
  window_.clear();
  window_.setView(camera_);
  window_.draw(*backgroundSprite_);
  window_.draw(*player_.sprite_);

  for (auto &building : buildings_)
  {
    if (building.sprite_)
    {
      window_.draw(*building.sprite_);
    }
  }
  for (auto &enemy : enemies_)
  {
    if (enemy.sprite_)
    {
      window_.draw(*enemy.sprite_);
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
