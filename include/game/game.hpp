#ifndef GAME
#define GAME

#include "../include/game/gameState.hpp"
#include "../include/engine/config.hpp"

class Camera;
class Tile;
class Dice;
class GameState;
class Model;

namespace game {
  struct SetupConfig {
    int oya;
    int playerCount;
    int seat;
    int tileSet;
    const Model* tileModel;
  };
};


class Input;

class Game {
public:
  Game();
  ~Game();

  void setupConfig(const game::SetupConfig& config);
  void setupCameras();
  void setupDie();

  void setup(const game::SetupConfig& config);

  void update(const Input& input);
  void gameUpdate(const Input& input);

  std::unique_ptr<Camera> cameras[global::cameraAmount];
  int currCamera;

  std::unique_ptr<Dice> die[global::diceAmount];
  std::vector<std::unique_ptr<Tile>> tiles;

  GameState lastGameState;

private:
  int oya;
  int playerCount;
  int seat;


  void switchCamera(const Input& input);
};

#endif
