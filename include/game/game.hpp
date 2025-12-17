#ifndef GAME
#define GAME

#include "../include/game/gameState.hpp"

class Camera;
class Tile;
class Dice;

class Model;

namespace game {
  struct SetupConfig {
    int oya;
    int playerCount;
    int seat;
    int tileSet;
    const Model& tileModel;
  };
};


class Input;

class Game {
public:
  Game();

  void setupConfig(const game::SetupConfig& config);
  void setupCameras();
  void setupDie();

  void setup(const game::SetupConfig& config);

  void update(const Input& input);

  std::vector<Tile>& getTiles();
  // const std::vector<Tile>& getTiles() const;

  std::unique_ptr<Camera> cameras[5];
  int currCamera;

private:
  int oya;
  int playerCount;
  int seat;

  std::vector<Tile> tiles;
  std::unique_ptr<Dice> die[2];

  gameState::GameState lastGameState;

  void switchCamera(const Input& input);
};

#endif
