#include "Azure.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace AzureTower;

AzureTower::Game::Game() : window_{sf::VideoMode({800, 600}), "Azure Tower"}
{
  window_.setKeyRepeatEnabled(false);
  window_.setFramerateLimit(60);

  if (!texture_.loadFromFile(PLAYER_TEXTURE_PATH))
  {
    std::cerr << "Failed to load texture from: " << PLAYER_TEXTURE_PATH << '\n';
  }

  // TODO - Init method for sprite and enemy?
  sprite_.emplace(texture_);
  sprite_->setPosition({50.f, 100.f});
  sprite_->setScale({2.f, 2.f});
  sf::FloatRect bounds = sprite_->getLocalBounds();
  sprite_->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

  testBlock_.setSize({50.f, 50.f});
  testBlock_.setPosition({400, 400});
  testBlock_.setFillColor(sf::Color::White);
  testBlock_.setOrigin(testBlock_.getGeometricCenter());
  Enemy slime;
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
          std::cout << sprite_->getPosition().x << " || " << sprite_->getPosition().y << std::endl;
        default: break;
      }
    }
  }
}

void AzureTower::Game::Update()
{
  // sf::Vector2i mousePos = sf::Mouse::getPosition(window_);

  sf::Time elapsed = clock_.getElapsedTime();
  float deltaTime = clock_.restart().asSeconds();

  sf::Vector2f movement{0.f, 0.f};

  // Check which axes have significant knockback velocity
  float velocityThreshold = 50.0f;
  bool knockbackX = std::abs(velocity_.x) > velocityThreshold;
  bool knockbackY = std::abs(velocity_.y) > velocityThreshold;

  // Gather input - only block movement on axes with active knockback
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && !knockbackY)
    movement.y -= player_.speed * deltaTime;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && !knockbackY)
    movement.y += player_.speed * deltaTime;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !knockbackX)
    movement.x -= player_.speed * deltaTime;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && !knockbackX)
    movement.x += player_.speed * deltaTime;

  // Apply velocity (sliding/knockback)
  sprite_->move(velocity_ * deltaTime);

  // Apply friction to slow down velocity over time
  float friction = 5.0f; // Higher = stops faster
  velocity_.x *= (1.0f - friction * deltaTime);
  velocity_.y *= (1.0f - friction * deltaTime);

  // Stop velocity if it's very small
  if (std::abs(velocity_.x) < 1.0f)
    velocity_.x = 0.f;
  if (std::abs(velocity_.y) < 1.0f)
    velocity_.y = 0.f;

  // Apply player input movement
  sprite_->move(movement);

  sf::Vector2f originalPos = sprite_->getPosition();

  // Check collision after movement
  auto playerBox = sprite_->getGlobalBounds();
  auto enemyBox = testBlock_.getGlobalBounds();

  if (playerBox.findIntersection(enemyBox))
  {
    // Collision detected - revert position
    sprite_->setPosition(originalPos);

    // Calculate knockback direction using vector subtraction
    sf::Vector2f playerPos = sprite_->getPosition();
    sf::Vector2f enemyPos = testBlock_.getPosition();
    sf::Vector2f knockbackDirection = playerPos - enemyPos; // Vector from enemy to player

    // Normalize the direction vector
    float length = std::sqrt(knockbackDirection.x * knockbackDirection.x + knockbackDirection.y * knockbackDirection.y);
    if (length > 0.001f)
    {
      knockbackDirection /= length; // Make it a unit vector
      
      // TODO make velocity and knockback strength specific to entity.
      // Apply knockback velocity
      velocity_ = knockbackDirection * slime_.knockBackStrength;
    }
  }
}

void AzureTower::Game::Render()
{
  window_.clear();
  if (sprite_)
  {
    window_.draw(testBlock_);
    window_.draw(*sprite_);
  }
  window_.display();
}

void Game::run()
{
}

bool Game::IsRunning() const
{
  return window_.isOpen();
}

bool AzureTower::Game::CollisionCheck()
{
  return false;
}

void Game::Knockback()
{
  /*
    time = deltatime; // get deltatime.
    movement = collect which direction the player is going.
    enemyMovement = collect which direct the enemy is going.

    velocity = collect at what speed the player is being pushed at.
    - each enemy has an attribute which determines how much velocity they apply.
    - velocity from their attacks (if any)
    - velocity from just colliding with their bounding box.

    detect collision
    - collision from enemy walking into you or you walking into them.
    - - impart their bounding box velocity value.
    - - collision from enemy hitting you or you hitting them.
    - - impart their velocity from their weapon or from your weapon to them.

    if there's been a collision via velocity > 0;
    - return player to original position // stop the movement
    - push player back at the speed of the velocity (over time via friction)
    - Make sure the player is being pushed back relative to the position of the enemy.
    - playerPos vs enemyPos?






  */
}
