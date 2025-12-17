#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

class TextureManager
{
private:
    std::unordered_map<std::string, sf::Texture> textures_;

public:
    sf::Texture &load(const std::string &name);
};
