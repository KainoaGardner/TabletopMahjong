#ifndef UPDATE_H
#define UPDATE_H

class EngineContext;
class Game;

namespace update {
  void update(EngineContext& engineCTX, Game& gameCTX);
  void gameUpdate(EngineContext& engineCTX, Game& gameCTX);

  void selectTile(EngineContext& engineCTX, Game& gameCTX);
}

#endif
