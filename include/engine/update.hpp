#ifndef UPDATE_H
#define UPDATE_H

#include "../include/engine/config.hpp"

class EngineContext;
class Game;

namespace update {
  void update(EngineContext& engineCTX, Game& gameCTX);
  void gameUpdate(EngineContext& engineCTX, Game& gameCTX);

  void selectTile(EngineContext& engineCTX, Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin);
  void unselectPlayerTiles(global::players player, Game& gameCTX);

  void updateHands(Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin);
}

#endif
