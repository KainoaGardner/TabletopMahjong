#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../include/camera.hpp"

#include <memory>

namespace gameState {

struct GameState {
  // Camera camera;
};


void setGameState(std::shared_ptr<GameState> gameState);
void swapGameState(std::shared_ptr<GameState> prevState, std::shared_ptr<GameState> currState);

void setup();
  
extern std::shared_ptr<GameState> currState;
extern std::shared_ptr<GameState> prevState;
}

#endif
