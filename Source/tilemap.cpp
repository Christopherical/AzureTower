#include "tilemap.hpp"
#include <SFML/graphics.hpp>
#include <iostream>

bool TileMap::load(const std::filesystem::path &tileset, sf::Vector2u tileSize, const int *tiles, unsigned int width,
                   unsigned int height)
{
  // load the tileset texture
  if (!m_tileset.loadFromFile(tileset))
    return false;

  // resize the vertex array to fit the level size
  m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
  m_vertices.resize(width * height * 6);

  // populate the vertex array, with two triangles per tile
  for (unsigned int i = 0; i < width; ++i)
  {
    for (unsigned int j = 0; j < height; ++j)
    {
      // get the current tile number
      const int tileNumber = tiles[i + j * width];

      // find its position in the tileset texture
      const int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
      const int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

      // Add spacing/margin adjustments if your tileset has gaps
      const int spacing = 0; // pixels between tiles
      const int margin = 0;  // pixels from edge of tileset

      // get a pointer to the triangles' vertices of the current tile
      sf::Vertex *triangles = &m_vertices[(i + j * width) * 6];

      // define the 6 corners of the two triangles
      // Triangle 1 (top-left half of square)
      triangles[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);       // Triangle 1: Top-left corner
      triangles[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y); // Triangle 1: Top-right corner
      triangles[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y); // Triangle 1: Bottom-left corner
      // Triangle 2 (bottom-right half of square)
      triangles[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y); // Triangle 2: Bottom-left corner
      triangles[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y); // Triangle 2: Top-right corner
      triangles[5].position =
          sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y); // Triangle 2: Bottom-right corner

    //   // define the 6 matching texture coordinates
    //   const float texLeft = margin + tu * (tileSize.x + spacing);
    //   const float texTop = margin + tv * (tileSize.y + spacing);
    //   const float texRight = texLeft + tileSize.x;
    //   const float texBottom = texTop + tileSize.y;

    //   triangles[0].texCoords = sf::Vector2f(texLeft, texTop);
    //   triangles[1].texCoords = sf::Vector2f(texRight, texTop);
    //   triangles[2].texCoords = sf::Vector2f(texLeft, texBottom);
    //   triangles[3].texCoords = sf::Vector2f(texLeft, texBottom);
    //   triangles[4].texCoords = sf::Vector2f(texRight, texTop);
    //   triangles[5].texCoords = sf::Vector2f(texRight, texBottom);

      // define the 6 matching texture coordinates
      triangles[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
      triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
      triangles[2].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
      triangles[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
      triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
      triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
    }
  }

  return true;
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  // apply the transform
  states.transform *= getTransform();

  // apply the tileset texture
  states.texture = &m_tileset;

  // draw the vertex array
  target.draw(m_vertices, states);
}
