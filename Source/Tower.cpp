#include "Tower.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

AzureTower::Tower::Tower(int zone, std::string name, sf::Vector2f position, sf::Texture &texture)
    : zone_(zone),
      name_(name),
      position_(position)
{
  sprite_.emplace(texture);
  sprite_->setPosition(position_);
  sprite_->setOrigin(sprite_->getLocalBounds().size / 2.f);

  attackRange_.setRadius(400);
  attackRange_.setOrigin(attackRange_.getGeometricCenter());
  attackRange_.setPosition(position);
  attackRange_.setFillColor(sf::Color(0, 255, 255, 50));
}

// void AzureTower::Tower::ToggleAttackRange()
// {
//   // TODO - Add Logging.
//   if(AttackRangeOn_){
//     AttackRangeOn_ = false;
//   }
//   else{
//     AttackRangeOn_ = true;
//   }
// }
