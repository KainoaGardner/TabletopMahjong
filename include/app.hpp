#ifndef APP_H
#define APP_H

#include "../include/engine/engineContext.hpp"
#include "../include/game/game.hpp"

class EngineContext;
class Game;

class App {
public:
  EngineContext engineCTX;
  Game gameCTX;

  void update();
  void render();
  void setup();
private:
};

#endif
