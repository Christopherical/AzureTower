#include "Building.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

AzureTower::Building::Building(int zone, std::string name, sf::Vector2f position, sf::Texture &texture)
    : zone_(zone),
      name_(name),
      position_(position)
{
  sprite_.emplace(texture);
  sprite_->setScale({3.f, 3.f});
  sprite_->setPosition(position_);
  sprite_->setOrigin(sprite_->getLocalBounds().size / 2.f);

  attackRange_.setRadius(600);
  attackRange_.setOrigin(attackRange_.getGeometricCenter());
  attackRange_.setPosition(position);
  attackRange_.setFillColor(sf::Color(0, 255, 255, 50));

  // buildingShoot();
}

void AzureTower::Building::buildingShoot(sf::Vector2f direction)
{
  projectileDirection_ = direction;
  auto timer = projectileClock_.getElapsedTime().asSeconds();
  if(timer > 2){

    // sf::RectangleShape projectile;
    projectile_.setSize({40.f, 60.f});
    projectile_.setOrigin(projectile_.getGeometricCenter());
    projectile_.setPosition(position_);
    projectile_.setFillColor(sf::Color::Red);
    projectileClock_.restart();
  }

}
