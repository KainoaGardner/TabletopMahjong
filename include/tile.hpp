#ifndef TILE_H
#define TILE_H

#include <unordered_map>
#include <glm/glm.hpp>

#include "../include/model.hpp"

namespace tile {
  enum TileEnum {
    Man1,
    Man2,
    Man3,
    Man4,
    Man5,
    Man6,
    Man7,
    Man8,
    Man9,
    Sou1,
    Sou2,
    Sou3,
    Sou4,
    Sou5,
    Sou6,
    Sou7,
    Sou8,
    Sou9,
    Pin1,
    Pin2,
    Pin3,
    Pin4,
    Pin5,
    Pin6,
    Pin7,
    Pin8,
    Pin9,
    Ton,
    Nan,
    Sha,
    Pei,
    Haku,
    Hatsu,
    Chun,
    Man5A,
    Sou5A,
    Pin5A,
  };

extern std::unordered_map<int, glm::vec2> tileUV;
}

class Tile {
public:
  Tile(unsigned int tileIn, const Model* modelIn);

  void draw() const;

private:
  unsigned int tile;
  const Model* model;
};


namespace tile {
extern std::vector<Tile> tiles;
void setup(int type);

}

#endif
