#ifndef UPDATE_H
#define UPDATE_H

#include "../include/engine/config.hpp"

class EngineContext;
class Game;
class Tile;

namespace update {
  void update(EngineContext& engineCTX, Game& gameCTX);
  void gameUpdate(EngineContext& engineCTX, Game& gameCTX);

  void mouse(EngineContext& engineCTX, Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin, global::players player);
  void click(EngineContext& engineCTX, Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin, global::players player);
  void hold(EngineContext& engineCTX, Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin, global::players player);
  void release(EngineContext& engineCTX, Game& gameCTX, global::players player);

  void tileRotate(EngineContext& engineCTX, Game& gameCTX, global::players player);

  Tile* selectTile(EngineContext& engineCTX, Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin, bool& reselect, global::players player);
  void unselectPlayerTiles(global::players player, Game& gameCTX);

  void updateHands(Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin, global::players player);
}

#endif
