#pragma once
#include <SFML/Graphics.hpp>
namespace AzureTower
{
class Interactable
{
public:
  const std::string name_;
  const int amount_;
  const sf::Vector2f position_;
  std::optional<sf::Sprite> sprite_;
  Interactable(std::string name, int amount, sf::Vector2f position, sf::Texture &texture);
};
} // namespace AzureTower