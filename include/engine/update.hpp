#ifndef UPDATE_H
#define UPDATE_H

#include "../include/engine/config.hpp"

class EngineContext;
class Game;

namespace update {
  void update(EngineContext& engineCTX, Game& gameCTX);
  void gameUpdate(EngineContext& engineCTX, Game& gameCTX);

  void selectTile(EngineContext& engineCTX, Game& gameCTX);
  void unselectPlayerTiles(global::players player, Game& gameCTX);
}

#endif
