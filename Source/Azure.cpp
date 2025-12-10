#include "Azure.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace AzureTower;

AzureTower::Game::Game() : window_{sf::VideoMode({200, 200}), "Azure Tower"}, sprite_{texture_}
{
  if (!texture_.loadFromFile(PLAYER_TEXTURE_PATH))
  {
    std::cerr << "Failed to load texture from: " << PLAYER_TEXTURE_PATH << '\n';
  }
  sprite_.setTexture(texture_);
}

sf::Sprite Game::getSprite() const
{
  sf::Sprite sprite(texture_);
  sprite.setPosition({100.f, 100.f});
  sprite.setScale({5.f, 5.f});
  return sprite;
}

void Game::processEvents()
{
  while (const std::optional event = window_.pollEvent())
  {
    if (event->is<sf::Event::Closed>())
    {
      window_.close();
    }

    else if (auto *mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
    {
      switch (mouseEvent->button)
      {
        case sf::Mouse::Button::Left: std::cout << "Left Button Pressed\n"; break;
        case sf::Mouse::Button::Right: std::cout << "Right Button Pressed\n"; break;
        case sf::Mouse::Button::Middle: std::cout << "Middle Button Pressed\n"; break;
        default: break;
      }
    }

    else if (auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
    {
      switch (keyEvent->code)
      {
        case sf::Keyboard::Key::W: std::cout << "Move Up\n"; break;
        case sf::Keyboard::Key::A: std::cout << "Move Left\n"; break;
        case sf::Keyboard::Key::S: std::cout << "Move Down\n"; break;
        case sf::Keyboard::Key::D: std::cout << "Move Right\n"; break;
        default: break;
      }
    }
  }
}

void AzureTower::Game::Render()
{
  window_.clear();
  window_.draw(getSprite());
  window_.display();
}

void Game::run()
{
}

bool Game::IsRunning() const
{
  return window_.isOpen();
}