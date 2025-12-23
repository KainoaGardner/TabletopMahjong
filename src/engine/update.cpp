#include "../include/engine/update.hpp"
#include "../include/engine/engineContext.hpp"

#include <GLES3/gl3.h>

#include "../include/game/game.hpp"
#include "../include/engine/engineContext.hpp"


// #include "../include/game/gameState.hpp"


void update(EngineContext& engineCTX, Game& gameCTX){
  gameCTX.update(engineCTX.input);

  engineCTX.input.update();
  engineCTX.input.actionPrev = engineCTX.input.actionCurr;

  gameCTX.lastGameState.createGameState(gameCTX);
}
