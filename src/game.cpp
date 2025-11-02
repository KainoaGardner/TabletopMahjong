#include "../include/game.hpp"

Game::Game() {

}

namespace game {
  
  std::unique_ptr<Game> game = std::make_unique<Game>();

void setup(){
}

}
