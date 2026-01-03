#include "../include/game/gameState.hpp"
#include "../include/game/game.hpp"
#include "../include/game/tile.hpp"
#include "../include/game/dice.hpp"
#include "../include/game/camera.hpp"
#include "../include/engine/config.hpp"

void GameState::createGameState(Game& gameCTX){
  createTileState(gameCTX);
  createDiceState(gameCTX);
  createCameraState(gameCTX);

  if (firstFrame){
    firstFrame = false;
  }
}

void GameState::createTileState(Game& gameCTX){
  tiles.clear();
  for (std::unique_ptr<Tile>& tile : gameCTX.tiles){
    gameState::TileState tileState = {
      .position = tile->position,
      .orientation = tile->orientation,
    };

    tiles.push_back(tileState);
  }
}

void GameState::createDiceState(Game& gameCTX){
  for (int i = 0; i < global::diceAmount; ++i){
    auto& dice = gameCTX.die[i];
    die[i].position = dice->position;
    die[i].orientation = dice->orientation;
  }
}

void GameState::createCameraState(Game& gameCTX){
  for (int i = 0; i < global::cameraAmount; ++i){
    auto& camera = gameCTX.cameras[i];
    cameras[i].position = camera->position;
    cameras[i].yaw = camera->yaw;
    cameras[i].pitch = camera->pitch;
    cameras[i].roll = camera->roll;
    cameras[i].fov = camera->fov;
  }
}
