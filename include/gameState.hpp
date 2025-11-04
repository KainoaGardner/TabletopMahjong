#ifndef GAME_STATE_H
#define GAME_STATE_H


#include "../include/camera.hpp"


namespace gameState {

struct GameState {
  Camera camera;
};


  
extern GameState currState;
extern GameState prevState;
}

#endif
