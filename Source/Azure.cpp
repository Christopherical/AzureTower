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
  sprite_->setPosition({50.f, 50.f});
  sprite_->setScale({2.f, 2.f});
  sf::FloatRect bounds = sprite_->getLocalBounds();
  sprite_->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

  testBlock_.setSize({50.f, 50.f});
  testBlock_.setPosition({400, 400});
  testBlock_.setFillColor(sf::Color::White);
  testBlock_.setOrigin(testBlock_.getGeometricCenter());
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
  sf::Time elapsed = clock_.getElapsedTime();
  float deltaTime = clock_.restart().asSeconds();

  sf::Vector2i mousePos = sf::Mouse::getPosition(window_);
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
  if (std::abs(velocity_.x) < 1.0f) velocity_.x = 0.f;
  if (std::abs(velocity_.y) < 1.0f) velocity_.y = 0.f;

  // Apply player input movement
  sprite_->move(movement);
  
  sf::Vector2f originalPos = sprite_->getPosition();

  // Check collision after movement
  auto playerBox = sprite_->getGlobalBounds();
  auto enemyBox = testBlock_.getGlobalBounds();

  if (playerBox.findIntersection(enemyBox))
  {
    // Collision detected - revert and apply bounce velocity
    sprite_->setPosition(originalPos);

    // Add knockback velocity (not instant movement)
    float knockbackStrength = 1000.0f; // Adjust for stronger/weaker slide
    sf::Vector2f knockbackDirection = -movement;
    
    // Normalize direction if there was movement
    float length = std::sqrt(knockbackDirection.x * knockbackDirection.x + knockbackDirection.y * knockbackDirection.y);
    if (length > 0.001f)
    {
      knockbackDirection /= length; // Normalize
      velocity_ = knockbackDirection * knockbackStrength; // Apply velocity
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
