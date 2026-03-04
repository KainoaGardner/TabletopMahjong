#ifndef TILE_H
#define TILE_H

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "../include/game/game.hpp"
#include "../include/engine/config.hpp"
#include <optional>

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

  enum TileSet {
    FourP,
    ThreeP,
  };


extern std::unordered_map<int, glm::vec2> tileUV;
}


class Model;
class Shader;

class Tile {
public:
  Tile(unsigned int tileIn, const Model* modelIn, glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn);

  glm::vec3 position;
  glm::quat orientation;
  glm::vec3 scale;
  glm::vec3 halfSize;

  std::optional<global::players> selected;

  void draw(Shader* shader) const;
  unsigned int tile;

  glm::mat4 getModelMatrix();


private:
  const Model* model;
};

namespace tile {

void setup(const game::SetupConfig& config, std::vector<std::unique_ptr<Tile>>& tiles);
void fourPSetup(std::vector<std::unique_ptr<Tile>>& tiles, const Model* tileModel);
void threePSetup(std::vector<std::unique_ptr<Tile>>& tiles, const Model* tileModel);

void shuffleTiles(std::vector<std::unique_ptr<Tile>>& tiles);
void flipTiles(std::vector<std::unique_ptr<Tile>>& tiles);
void makeWalls(std::vector<std::unique_ptr<Tile>>& tiles);
void dealHands(int roll, int oya, std::vector<std::unique_ptr<Tile>>& tiles);
void makeDeadWall(int roll, int oya, std::vector<std::unique_ptr<Tile>>& tiles);
}

#endif
