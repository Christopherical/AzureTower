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
}

void AzureTower::Building::buildingShoot()
{
}
