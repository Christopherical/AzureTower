#include "Azure.hpp"
#include "Enemy.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <string>

////////// TODO - Add different Enemy Types. //////////
////////// TODO - Add Cooldown for taking damage. //////////
////////// TODO - Add Weapon Swing into Enemy. //////////
////////// TODO - Add Game Over Screen. //////////
////////// TODO - Add Movement for Enemies. //////////
////////// TODO - Add Actual Background. //////////
////////// TODO - Add SPD Logging. //////////
////////// TODO - Add Music. //////////
////////// TODO - Add performance testing with lots of enemies. //////////
////////// TODO - Add enemy Pathfinding //////////
// ---NAVIGATION MESH. A*? (Multiple - Crown Simulation,
// flocking, float fields ,RVO (reciprocal veocity avoidance - good for smaller amounts)
// -- what is network prediction/rollback
////////// TODO - Add Diagonal movement adjustment 1.4 //////////

using namespace AzureTower;

AzureTower::Game::Game()
    : window_{sf::VideoMode({GAME_WIDTH, GAME_HEIGHT}), "Azure Tower"},
      font_{"C:\\Users\\Chris\\Desktop\\rpgPrototype\\AzureTower\\Content\\Fonts\\arial.ttf"}
{
  InitGame();
  InitPlayer();
}

void AzureTower::Game::InitGame()
{
  window_.setKeyRepeatEnabled(false);
  window_.setFramerateLimit(FRAME_RATE);
  camera_.setSize({CAMERA_WIDTH, CAMERA_HEIGHT});

  backgroundSprite_.emplace(textureManager_.load(BACKGROUND_NAME));
  backgroundSprite_->setPosition({0.f, 0.f});
  backgroundSprite_->setScale({5.f, 5.f});

  enemies_.reserve(MAX_ENEMIES);
}

void AzureTower::Game::InitPlayer()
{
  player_.sprite_.emplace(textureManager_.load(PLAYER_NAME));
  player_.sprite_->setScale(SPRITE_SCALE);
  player_.sprite_->setOrigin(player_.sprite_->getLocalBounds().size / 2.f);

  // Position at background center
  auto bgBounds = backgroundSprite_->getGlobalBounds();
  player_.sprite_->setPosition(bgBounds.position + bgBounds.size / 2.f);

  camera_.setCenter(player_.sprite_->getGlobalBounds().position);
}

void Game::ProcessEvents()
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
        case sf::Mouse::Button::Left:
          auto mouseWorldPos = window_.mapPixelToCoords(sf::Mouse::getPosition(window_), camera_);

          // Testing creation of buildings on mouse click.
          towers_.emplace_back(1, "house", mouseWorldPos, textureManager_.load("house"));

          // Testing clicking on enemies.
          for (auto &enemy : enemies_)
          {
            if (enemy.sprite_->getGlobalBounds().contains(mouseWorldPos))
            {
              enemy.sprite_->setColor(sf::Color::Blue);
              break;
            }
          }
      }
    }
    else if (auto *keyEvent = event->getIf<sf::Event::KeyPressed>())
    {
      // Handle one-time key presses here (menus, actions, etc.)
      switch (keyEvent->code)
      {
        case sf::Keyboard::Key::Escape: std::cout << "Escape pressed\n"; break;
        case sf::Keyboard::Key::Q:
          if (player_.sprite_)
            std::cout << player_.sprite_->getPosition().x << " || " << player_.sprite_->getPosition().y << std::endl;
          break;
        default: break;
      }
    }
    else if (auto *scrollEvent = event->getIf<sf::Event::MouseWheelScrolled>())
    {
      if (scrollEvent->wheel == sf::Mouse::Wheel::Vertical)
      {
        float zoomFactor = (scrollEvent->delta > 0) ? 0.9f : 1.1f;
        float newZoom = zoomLevel_ * zoomFactor;

        if (newZoom >= MIN_CAMERA_ZOOM && newZoom <= MAX_CAMERA_ZOOM)
        {
          zoomLevel_ = newZoom;
          camera_.zoom(zoomFactor);
        }
      }
    }
  }
}

void AzureTower::Game::Update()
{
  if (!gameOver_)
  {
    float deltaTime = clock_.restart().asSeconds();
    spawnSlime();

    // Player input & movement.
    sf::Vector2f movement{0.f, 0.f};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
      movement.y -= player_.speed_ * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
      movement.y += player_.speed_ * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
      movement.x -= player_.speed_ * deltaTime;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
      movement.x += player_.speed_ * deltaTime;
    player_.sprite_->move(movement);

    // Movement + Collision checks.
    for (auto &enemy : enemies_)
    {
      enemy.sprite_->move({0.f, (SLIME_SPEED * deltaTime)});
      EnemyCollisionCheck(enemy);
    }
    for (auto &projectile : projectiles_)
    {
      projectile.projectile_.move(projectile.direction_ * PROJECTILE_SPEED);
      ProjectileCollisionCheck(projectile);
    }
    for (auto &tower : towers_)
    {
      TowerCollisionCheck(tower);
    }

    // End of Frame updates - Removal, Camera set, Game over check.
    enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [](const Enemy &enemy) { return enemy.isDead_; }),
                   enemies_.end());
    projectiles_.erase(std::remove_if(projectiles_.begin(), projectiles_.end(),
                                      [](const Projectile &projectile) { return projectile.isDead_; }),
                       projectiles_.end());
    camera_.setCenter(player_.sprite_->getGlobalBounds().position);
    GameOver();
  }
}

void AzureTower::Game::EnemyCollisionCheck(Enemy &enemy)
{
  if (enemy.sprite_->getPosition().y > BOTTOM_THRESHOLD)
  {
    player_.health_ = player_.health_ - 50;
    enemy.isDead_ = true;
  }
}

void AzureTower::Game::ProjectileCollisionCheck(Projectile &projectile)
{
  for (auto &enemy : enemies_)
  {
    if (projectile.projectile_.getGlobalBounds().findIntersection(enemy.sprite_->getGlobalBounds()))
    {
      // TODO - Play Hit Sound.
      enemy.isDead_ = true;
      projectile.isDead_ = true;
    }
  }
  // TODO - Change from hardcoded.
  if (projectile.projectile_.getPosition().y > BOTTOM_THRESHOLD || projectile.projectile_.getPosition().x < 0 ||
      projectile.projectile_.getPosition().y < 0 || projectile.projectile_.getPosition().x > 1195)
  {
    projectile.isDead_ = true;
  }
}

void AzureTower::Game::TowerCollisionCheck(Tower &tower)
{
  if (tower.projectileClock_.getElapsedTime().asSeconds() > 0.5f)
  {
    tower.projectileClock_.restart();
    for (auto &enemy : enemies_)
    {
      // Check if enemy is within circular attack range
      sf::Vector2f towerPos = tower.sprite_->getPosition();
      sf::Vector2f enemyPos = enemy.sprite_->getPosition();
      float dx = enemyPos.x - towerPos.x;
      float dy = enemyPos.y - towerPos.y;
      float distance = std::sqrt(dx * dx + dy * dy);

      if (distance <= tower.attackRange_.getRadius())
      {
        sf::Vector2f direction = enemyPos - towerPos;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0.001f)
          tower.projectileDirection_ = direction / length;
        projectiles_.emplace_back(tower.projectileDirection_, towerPos);
        break;
      }
    }
  }
}

void AzureTower::Game::spawnSlime()
{
  if (enemyClock_.getElapsedTime().asSeconds() > 2.0f && enemies_.size() < MAX_ENEMIES)
  {
    enemies_.emplace_back(SLIME_NAME, SLIME_HEALTH, SLIME_SPEED, SLIME_POSITION, textureManager_.load(SLIME_NAME));
    enemyClock_.restart();
  }
}

void AzureTower::Game::GameOver()
{
  if (player_.health_ <= 0)
  {
    gameOverText_.emplace(font_);
    gameOverText_->setString("GAME OVER");
    gameOverText_->setScale({2.f, 2.f});
    gameOverText_->setFillColor(sf::Color::Red);
    gameOverText_->setOrigin(gameOverText_->getLocalBounds().size / 2.f);
    gameOver_ = true;
  }
}

void AzureTower::Game::Render()
{
  window_.clear();
  window_.setView(camera_);

  // Handle game over screen
  if (gameOver_ && gameOverText_)
  {
    gameOverText_->setPosition(camera_.getCenter());
    window_.draw(*gameOverText_);
    window_.display();
    return;
  }

  // Normal game rendering
  window_.draw(*backgroundSprite_);
  window_.draw(*player_.sprite_);

  for (auto &enemy : enemies_)
  {
    if (enemy.sprite_)
    {
      window_.draw(*enemy.sprite_);
    }
  }
  for (auto &tower : towers_)
  {
    if (tower.sprite_)
    {
      window_.draw(tower.attackRange_);
      window_.draw(*tower.sprite_);
    }
  }
  for (auto &projectile : projectiles_)
  {
    window_.draw(projectile.projectile_);
  }

  window_.display();
}

bool AzureTower::Game::IsRunning() const
{
  return window_.isOpen();
}
