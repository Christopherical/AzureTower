#include "Building.hpp"

#include <SFML/Graphics.hpp>

AzureTower::Building::Building(int zone, std::string name, sf::Vector2f position, sf::Texture &texture)
    : zone_(zone),
      name_(name),
      position_(position)
{
  sprite_.emplace(texture);
  sprite_->setScale({3.f, 3.f});
  sprite_->setPosition({600.f, 200.f});
}
