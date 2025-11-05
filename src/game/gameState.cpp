#include "../include/gameState.hpp"


namespace gameState {
  
std::shared_ptr<GameState> currState;
std::shared_ptr<GameState> prevState;

void setup(){
  currState = std::make_shared<GameState>();
  prevState = nullptr;
}

void setGameState(std::shared_ptr<GameState> gameState) {
}

void swapGameState(std::shared_ptr<GameState> prevState, std::shared_ptr<GameState> currState) {

}

}
