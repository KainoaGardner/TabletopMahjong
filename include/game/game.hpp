#ifndef GAME
#define GAME

#include "../include/game/gameState.hpp"
#include "../include/engine/config.hpp"

class Camera;
class Tile;
class Dice;
class GameState;
class Model;
class Hand;

namespace game {
  struct SetupConfig {
    int oya;
    int playerCount;
    int seat;
    int tileSet;
    const Model* tileModel;
  };

  //TEST update
  glm::vec3 getPlayerColor(global::players player);
};


class Input;

class Game {
public:
  Game();
  ~Game();

  void setupConfig(const game::SetupConfig& config);
  void setupCameras(const game::SetupConfig& config);
  void setupDie();
  void setupHands(const game::SetupConfig& config);

  void setup(const game::SetupConfig& config);

  void update(const Input& input);
  void gameUpdate(const Input& input);

  std::unique_ptr<Camera> cameras[global::cameraAmount];
  int currCamera;

  std::unique_ptr<Dice> die[global::diceAmount];
  std::unique_ptr<Hand> hands[global::handAmount];
  std::vector<std::unique_ptr<Tile>> tiles;

  GameState lastGameState;

  glm::vec3 click = glm::vec3(0.0f);

private:
  int oya;
  int playerCount;
  int seat;

  void switchCamera(const Input& input);
};

#endif
