#pragma once

#include <string>
#include <string_view>

#include <SFML/Graphics.hpp>

namespace AzureTower
{
// Game Constants.
const std::string BACKGROUND_NAME = "backgroundStoneLarge";
constexpr int GAME_WIDTH = 1000;
constexpr int GAME_HEIGHT = 1200;
constexpr float CAMERA_WIDTH = 1000.f;
constexpr float CAMERA_HEIGHT = 1200.f;
constexpr float MIN_CAMERA_ZOOM = 0.5f;
constexpr float MAX_CAMERA_ZOOM = 3.0f;
constexpr sf::Vector2f SPRITE_SCALE = {3.f, 3.f};
constexpr int FRAME_RATE = 60;
constexpr int MAX_ENEMIES = 100;
constexpr int BOTTOM_THRESHOLD = 2350;
constexpr int WEST_THRESHOLD = 2350;


// Player Constants.
const std::string PLAYER_NAME = "player2";
constexpr int PLAYER_HEALTH = 100;
constexpr int PLAYER_SPEED = 300;
constexpr int PLAYER_STARTING_GOLD = 500;

// Slime Constants.
const std::string SLIME_NAME = "slime";
constexpr int SLIME_HEALTH = 50;
constexpr float SLIME_SPEED = 50;
constexpr float SLIME_KNOCKBACK_STRENGTH = 1000.f;
constexpr sf::Vector2f SLIME_POSITION = {400.f, 0.f};

// Building Constants.
const std::string BUILDING_NAME = "house";
constexpr sf::Vector2f BUILDING_POSITION = {700.f, 400.f};
constexpr float PROJECTILE_SPEED = 30.f;

// Path Constants. (Avoiding concatenation so I can use constexpr + string_view)
constexpr std::string_view FONT_PATH = "Content\\Fonts\\arial.ttf";
constexpr std::string_view PLAYER_TEXTURE_PATH = "Content\\Textures\\player.png";
constexpr std::string_view SLIME_TEXTURE_PATH = "Content\\Textures\\slimeRo.png";
const std::string YELLOWBALL_TEXTURE_PATH = "Content\\Textures\\MediumYellowBall.png";
} // namespace AzureTower