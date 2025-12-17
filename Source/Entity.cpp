#include "Entity.hpp"
#include "AzureConfig.hpp"
#include "TextureManager.hpp"

AzureTower::Player::Player()
    : health_(PLAYER_HEALTH),
      speed_(PLAYER_SPEED),
      position_(sf::Vector2f(200.f, 200.f))
{
}

void AzureTower::Player::InitPlayer()
{
  sprite_->setPosition({50.f, 100.f});
  sprite_->setScale(SPRITE_SCALE);
  sf::FloatRect bounds = sprite_->getLocalBounds();
  sprite_->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
};

AzureTower::Enemy::Enemy(std::string name, int health, int speed, sf::Vector2f position, sf::Texture &texture)
    : name_(name),
      health_(health),
      speed_(speed),
      position_(position)
{
  sprite_.emplace(texture);
}
