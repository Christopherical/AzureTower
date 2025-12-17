#include "Interactable.hpp"

AzureTower::Interactable::Interactable(std::string name, int amount, sf::Vector2f position, sf::Texture &texture)
    : name_(name),
    amount_(amount),
    position_(position)
{
    sprite_.emplace(texture);
}