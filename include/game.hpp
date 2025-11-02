#ifndef GAME_H
#define GAME_H

#include <memory>

class Game {
public:
  Game();

  int oya = 0;
  int playerCount = 4;
private:
};


namespace game {

void setup();

  
extern std::unique_ptr<Game> game;
}

#endif
