
// Add functions for potentially needed classes
// void Game::Knockback()
// {
//   // sf::Vector2i mousePos = sf::Mouse::getPosition(window_);

//   sf::Time elapsed = clock_.getElapsedTime();
//   float deltaTime = clock_.restart().asSeconds();

//   sf::Vector2f movement{0.f, 0.f};

//   // Check which axes have significant knockback velocity
//   float velocityThreshold = 50.0f;
//   bool knockbackX = std::abs(velocity_.x) > velocityThreshold;
//   bool knockbackY = std::abs(velocity_.y) > velocityThreshold;

//   // Gather input - only block movement on axes with active knockback
//   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && !knockbackY)
//     movement.y -= player_.speed * deltaTime;
//   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && !knockbackY)
//     movement.y += player_.speed * deltaTime;
//   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && !knockbackX)
//     movement.x -= player_.speed * deltaTime;
//   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && !knockbackX)
//     movement.x += player_.speed * deltaTime;

//   // Apply velocity (sliding/knockback)
//   player_.sprite->move(velocity_ * deltaTime);

//   // Apply friction to slow down velocity over time
//   float friction = 5.0f; // Higher = stops faster
//   velocity_.x *= (1.0f - friction * deltaTime);
//   velocity_.y *= (1.0f - friction * deltaTime);

//   // Stop velocity if it's very small
//   if (std::abs(velocity_.x) < 1.0f)
//     velocity_.x = 0.f;
//   if (std::abs(velocity_.y) < 1.0f)
//     velocity_.y = 0.f;

//   // Apply player input movement
//   player_.sprite->move(movement);

//   sf::Vector2f originalPos = player_.sprite->getPosition();

//   // Check collision after movement
//   auto playerBox = player_.sprite->getGlobalBounds();
//   auto enemyBox = slime_.sprite->getGlobalBounds();

//   if (playerBox.findIntersection(enemyBox))
//   {
//     // Collision detected - revert position
//     player_.sprite->setPosition(originalPos);

//     // Calculate knockback direction using vector subtraction
//     sf::Vector2f playerPos = player_.sprite->getPosition();
//     sf::Vector2f enemyPos = slime_.sprite->getPosition();
//     sf::Vector2f knockbackDirection = playerPos - enemyPos; // Vector from enemy to player

//     // Normalize the direction vector
//     float length = std::sqrt(knockbackDirection.x * knockbackDirection.x + knockbackDirection.y *
//     knockbackDirection.y); if (length > 0.001f)
//     {
//       knockbackDirection /= length; // Make it a unit vector

//       // TODO make velocity and knockback strength specific to entity.
//       // Apply knockback velocity
//       velocity_ = knockbackDirection * slime_.knockBackStrength;
//     }
//   }
// }
