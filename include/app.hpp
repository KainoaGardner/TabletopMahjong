#ifndef APP_H
#define APP_H

#include "engine/engineContext.hpp"

#include "game/game.hpp"

class EngineContext;
class Game;

class App {
public:
  EngineContext engineCTX;
  Game gameCTX;

  void update();
  void gameUpdate();
  void render(float a);
  void setup();
private:
};

#endif
