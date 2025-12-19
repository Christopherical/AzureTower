#include <string>

#include "AzureConfig.hpp"
#include "Enemy.hpp"
#include "TextureManager.hpp"

AzureTower::Enemy::Enemy(std::string name, int health, int speed, sf::Vector2f position, sf::Texture &texture)
    : name_(name),
      health_(health),
      speed_(speed)
{
  sprite_.emplace(texture);
  sprite_->setPosition(position);
  sprite_->setScale({3.f, 3.f});
}

sf::Vector2f AzureTower::Enemy::Movement2(float deltaTime){

  return sf::Vector2f{0.f,(SLIME_SPEED * deltaTime)};

}
