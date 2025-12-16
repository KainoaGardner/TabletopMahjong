#ifndef GAME
#define GAME

#include "../include/game/gameState.hpp"

class Camera;
class Tile;
class Dice;

namespace game {
  struct SetupConfig {
    int oya;
    int playerCount;
    int seat;
  };
};


class Input;

class Game {
public:
  Game();

  void setupConfig(game::SetupConfig config);
  void setupCameras();
  void setupTiles();
  void setupDie();

  void setup(game::SetupConfig config);

  void update(const Input& input);

private:
  int oya;
  int playerCount;
  int seat;
  int currCamera;

  std::unique_ptr<Camera> cameras[5];
  std::vector<Tile> tiles;
  std::vector<Dice> die;

  gameState::GameState lastGameState;

  void switchCamera(const Input& input);
};

#endif
