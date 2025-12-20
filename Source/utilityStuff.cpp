//   // Check collision after movement
//   auto playerBox = player_.sprite_->getGlobalBounds();
//   for (auto &enemy : enemies_)
//   {
//     auto enemyBox = enemy.sprite_->getGlobalBounds();
//     if (playerBox.findIntersection(enemyBox))
//     {
//       player_.health_ -= 10;
//       player_.sprite_->setPosition(originalPos);
//     }
//   }

//   for (auto &building : buildings_)
//   {
//     auto buildingBox = building.sprite_->getGlobalBounds();

//     // Remove roof from collision
//     float roofHeight = 60.f;
//     buildingBox.position.y += roofHeight;
//     buildingBox.size.y -= roofHeight;

//     if (playerBox.findIntersection(buildingBox))
//     {
//       player_.sprite_->setPosition(originalPos);
//     }
//   }