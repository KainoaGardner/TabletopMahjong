#include "../include/engine/update.hpp"
#include "../include/engine/engineContext.hpp"

#include <GLES3/gl3.h>

#include "../include/game/game.hpp"
#include "../include/engine/engineContext.hpp"
#include "../include/game/camera.hpp"


#include <iostream>

namespace update {

void gameUpdate(EngineContext& engineCTX, Game& gameCTX){
  gameCTX.gameUpdate(engineCTX.input);

  gameCTX.lastGameState.createGameState(gameCTX);
}

void update(EngineContext& engineCTX, Game& gameCTX){
  engineCTX.input.update();

  gameCTX.update(engineCTX.input);

  engineCTX.input.actionPrev = engineCTX.input.actionCurr;
  engineCTX.input.clear();
}

}
