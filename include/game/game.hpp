#ifndef GAME
#define GAME

#include "game/gameState.hpp"
#include "engine/config.hpp"

class Camera;
class Tile;
class Dice;
class GameState;
class Model;
class Hand;
class LockSpace;

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



  void setup(const game::SetupConfig& config);

  void update(const Input& input);
  void gameUpdate(const Input& input);

  std::unique_ptr<Camera> cameras[global::cameraAmount];
  int currCamera;

  std::unique_ptr<Dice> die[global::diceAmount];
  std::unique_ptr<Hand> hands[global::handAmount];
  std::vector<std::unique_ptr<Tile>> tiles;

  std::unique_ptr<LockSpace> handLockSpaces[global::handLockSpaceAmount];
  std::unique_ptr<LockSpace> discardLockSpaces[global::discardLockSpaceAmount];
  std::unique_ptr<LockSpace> callLockSpaces[global::callLockSpaceAmount];
  std::unique_ptr<LockSpace> yamaLockSpaces[global::yamaLockSpaceAmount];

  GameState lastGameState;

private:
  int oya;
  int playerCount;
  int seat;

  void switchCamera(const Input& input);

  void setupConfig(const game::SetupConfig& config);
  void setupCameras(const game::SetupConfig& config);
  void setupDie();
  void setupHands(const game::SetupConfig& config);
  void setupLockSpaces(int tileSet);
  void setupHandLockSpaces(int tileSet);
  void setupDiscardLockSpaces(int tileSet);
  void setupCallLockSpaces(int tileSet);
  void setupYamaLockSpaces(int tileSet);
};

#endif
