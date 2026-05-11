#include "game/game.hpp"
#include "engine/model.hpp"
#include "game/camera.hpp"
#include "game/dice.hpp"
#include "game/tile.hpp"
#include "game/hand.hpp"
#include "game/lockon.hpp"

#include "engine/input.hpp"
#include "engine/config.hpp"

#include <random>
#include <iostream>

namespace game {
glm::vec3 getPlayerColor(global::players player){
  switch(player) {
    case global::shimocha:
      return global::colorToVec3[global::colors::blue];
    case global::toimen:
      return global::colorToVec3[global::colors::red];
    case global::kamicha:
      return global::colorToVec3[global::colors::green];
    default:
      return global::colorToVec3[global::colors::yellow];
  }

  return glm::vec3(0.0);
}
};

Game::Game(){}

Game::~Game() = default;

void Game::gameUpdate(const Input& input){
  auto& camera = cameras[currCamera];
  bool topCamera = currCamera == camera::CameraEnum::top;
  camera->gameUpdate(input, topCamera);
}

void Game::update(const Input& input){
  switchCamera(input);

  auto& camera = cameras[currCamera];
  bool topCamera = currCamera == camera::CameraEnum::top;
  camera->update(input, topCamera);
}

void Game::switchCamera(const Input& input){
  int initial = currCamera;

  if (input.justPressed(input::actions::mainCamera)){
    currCamera = seat;
  }else if (input.justPressed(input::actions::pointCamera)){
    currCamera = seat;
  }else if (input.justPressed(input::actions::topCamera)){
    currCamera = camera::CameraEnum::top;
  }

  //remove
  if (input.justPressed(input::actions::camera1)){
    currCamera = camera::CameraEnum::ton1;
  }else if (input.justPressed(input::actions::camera2)){
    currCamera = camera::CameraEnum::nan1;
  }else if (input.justPressed(input::actions::camera3)){
    currCamera = camera::CameraEnum::sha1;
  }else if (input.justPressed(input::actions::camera4)){
    currCamera = camera::CameraEnum::pei1;
  }

  if (initial != currCamera){
    bool topCamera = currCamera == camera::CameraEnum::top;
    cameras[currCamera]->revert(topCamera);
  }
}

void Game::setupCameras(const game::SetupConfig& config){
  cameras[0] = std::make_unique<Camera>(
    camera::tonPos, camera::yaw, camera::pitch, camera::roll, camera::fov, camera::speed, camera::moveSpeed, camera::sensitivity, camera::zoomSpeed);

  cameras[1] = std::make_unique<Camera>(
    camera::nanPos, camera::yaw + 90.0f, camera::pitch, camera::roll, camera::fov, camera::speed, camera::moveSpeed, camera::sensitivity, camera::zoomSpeed);

  cameras[2] = std::make_unique<Camera>(
    camera::shaPos, camera::yaw + 180.0f, camera::pitch, camera::roll, camera::fov, camera::speed, camera::moveSpeed, camera::sensitivity, camera::zoomSpeed);

  cameras[3] = std::make_unique<Camera>(
    camera::peiPos, camera::yaw + 270.0f, camera::pitch, camera::roll, camera::fov, camera::speed, camera::moveSpeed, camera::sensitivity, camera::zoomSpeed);

  cameras[4] = std::make_unique<Camera>(
    camera::topPos, camera::yaw, -90.0f, camera::roll, camera::fov, camera::speed, camera::moveSpeed, camera::sensitivity, camera::zoomSpeed);

}

void Game::setupDie(){
  die[0] = std::make_unique<Dice>();
  die[1] = std::make_unique<Dice>();

  // int roll = dice::getDiceRoll() + dice::getDiceRoll();
}

void Game::setupLockSpaces(int tileSet){
  // glm::quat orientation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  // handLockSpaces[0] = std::make_unique<LockSpace>(glm::vec3(0.25f, model::tileScale.y / 2.0f, 0.0f), orientation, glm::vec3(model::tileScale));
  setupDiscardLockSpaces(tileSet);
  setupCallLockSpaces(tileSet);

  // setupHandLockSpaces(tileSet);
  // setupYamaLockSpaces(tileSet);
}

// void Game::setupHandLockSpaces(int tileSet){
//   int walls;
//   if (tileSet == tile::TileSet::ThreeP){
//     walls = 3;
//   }else {
//     walls = 4;
//   }
//
//   float r = model::handRadiusDistance;
//   float startY = 0.0;
//   float offset = -model::tileScale.x * (13.0f / 2.0f - 0.5f);
//
//   for (int i = 0; i < 13 * walls; i++){
//     int x = i % 13;
//     int z = (i / 13); 
//
//     glm::vec3 pos = glm::vec3(0.0f);
//     pos.y = startY;
//
//     int xMove = std::cos(glm::radians(90.0f * z));
//     int zMove = -std::sin(glm::radians(90.0f * z));
//     pos.x = r * xMove; 
//     pos.z = r * zMove;
//
//     pos.x += offset * zMove; 
//     pos.z += -offset * xMove;
//
//     pos.x += x * model::tileScale.x * zMove; 
//     pos.z -= x * model::tileScale.x * xMove;
//
//     // glm::quat o1 = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//     glm::quat orientation = glm::angleAxis(glm::radians(90.0f * (z + 1)), glm::vec3(0.0f, 1.0f, 0.0f));
//     handLockSpaces[i] = std::make_unique<HandLockSpace>(pos, orientation, glm::vec3(model::tileScale.x, model::tileScale.y * lock::spaceHeightFactor, model::tileScale.z));
//   }
//
//   for (int i = 0; i < walls; i++){
//     float x = 13.5f;
//     int z = i; 
//
//     glm::vec3 pos = glm::vec3(0.0f);
//     pos.y = startY;
//
//     int xMove = std::cos(glm::radians(90.0f * z));
//     int zMove = -std::sin(glm::radians(90.0f * z));
//     pos.x = r * xMove; 
//     pos.z = r * zMove;
//
//     pos.x += offset * zMove; 
//     pos.z += -offset * xMove;
//
//     pos.x += x * model::tileScale.x * zMove; 
//     pos.z -= x * model::tileScale.x * xMove;
//
//     glm::quat orientation = glm::angleAxis(glm::radians(90.0f * (z + 1)), glm::vec3(0.0f, 1.0f, 0.0f));
//     handLockSpaces[52 + z] = std::make_unique<HandLockSpace>(pos, orientation, glm::vec3(model::tileScale.x, model::tileScale.y * lock::spaceHeightFactor, model::tileScale.z));
//   }
// }

void Game::setupDiscardLockSpaces(int tileSet){
  int walls;
  if (tileSet == tile::TileSet::ThreeP){
    walls = 3;
  }else {
    walls = 4;
  }

  float r = model::discardRadiusDistance;
  float startY = 0.0;
  float offset = -model::tileScale.x * 2.5f;

  for (int i = 0; i < 24 * walls; i++){
    int x = i % 6;
    int z = (i / 24); 
    int y = (i % 24) / 6;

    glm::vec3 pos = glm::vec3(0.0f);
    pos.y = startY;

    int xMove = std::cos(glm::radians(90.0f * z));
    int zMove = -std::sin(glm::radians(90.0f * z));
    pos.x = r * xMove; 
    pos.z = r * zMove;

    pos.x += offset * zMove; 
    pos.z += -offset * xMove;

    pos.x += x * model::tileScale.x * zMove; 
    pos.z -= x * model::tileScale.x * xMove;

    pos.x += y * model::tileScale.z * xMove; 
    pos.z += y * model::tileScale.z * zMove;

    glm::quat orientation = glm::angleAxis(glm::radians(90.0f * (z + 1)), glm::vec3(0.0f, 1.0f, 0.0f));
    discardLockSpaces[i] = std::make_unique<DiscardLockSpace>(pos, orientation, glm::vec3(model::tileScale.x, model::tileScale.y * lock::spaceHeightFactor, model::tileScale.z));
  }
}

void Game::setupCallLockSpaces(int tileSet){
  int walls;
  if (tileSet == tile::TileSet::ThreeP){
    walls = 3;
  }else {
    walls = 4;
  }

  float r = model::callRadiusDistance;
  float startY = 0.0;
  float offset = model::matScale.x * 0.5f - model::tileScale.x * 15.5f;

  for (int i = 0; i < 16 * walls; i++){
    int x = i % 16;
    int z = (i / 16); 

    glm::vec3 pos = glm::vec3(0.0f);
    pos.y = startY;

    int xMove = std::cos(glm::radians(90.0f * z));
    int zMove = -std::sin(glm::radians(90.0f * z));
    pos.x = r * xMove; 
    pos.z = r * zMove;

    pos.x += offset * zMove; 
    pos.z += -offset * xMove;

    pos.x += x * model::tileScale.x * zMove; 
    pos.z -= x * model::tileScale.x * xMove;

    glm::quat orientation = glm::angleAxis(glm::radians(90.0f * (z + 1)), glm::vec3(0.0f, 1.0f, 0.0f));
    callLockSpaces[i] = std::make_unique<CallLockSpace>(pos, orientation, glm::vec3(model::tileScale.x, model::tileScale.y * lock::spaceHeightFactor, model::tileScale.z));
  }


  offset = model::matScale.x * 0.5f - model::tileScale.x * 3.5f;
  for (int i = 0; i < 4 * walls; i++){
    int x = i % 4;
    int z = (i / 4); 

    glm::vec3 pos = glm::vec3(0.0f);
    pos.y = startY;

    int xMove = std::cos(glm::radians(90.0f * z));
    int zMove = -std::sin(glm::radians(90.0f * z));
    pos.x = r * xMove; 
    pos.z = r * zMove;

    pos.x += offset * zMove; 
    pos.z += -offset * xMove;

    pos.x += x * model::tileScale.x * zMove; 
    pos.z -= x * model::tileScale.x * xMove;

    pos.x -= model::tileScale.z * xMove; 
    pos.z -= model::tileScale.z * zMove;

    glm::quat orientation = glm::angleAxis(glm::radians(90.0f * (z + 1)), glm::vec3(0.0f, 1.0f, 0.0f));
    callLockSpaces[16 * 4 + i] = std::make_unique<CallLockSpace>(pos, orientation, glm::vec3(model::tileScale.x, model::tileScale.y * lock::spaceHeightFactor, model::tileScale.z));
  }
}

void Game::setupHands(const game::SetupConfig& config){
  hands[0] = std::make_unique<Hand>(glm::vec3(0.0f), global::colorToVec3[global::colors::yellow]);
  hands[1] = std::make_unique<Hand>(glm::vec3(0.0f), global::colorToVec3[global::colors::blue]);
  hands[2] = std::make_unique<Hand>(glm::vec3(0.0f), global::colorToVec3[global::colors::red]);
  hands[3] = std::make_unique<Hand>(glm::vec3(0.0f), global::colorToVec3[global::colors::green]);
}



void Game::setupConfig(const game::SetupConfig& config){
  oya = config.oya;
  playerCount = config.playerCount;
  seat = config.seat;

  switch (seat) {
    case camera::CameraEnum::ton1:
      currCamera = camera::CameraEnum::ton1;
      break;
    case camera::CameraEnum::nan1:
      currCamera = camera::CameraEnum::nan1;
      break;
    case camera::CameraEnum::sha1:
      currCamera = camera::CameraEnum::sha1;
      break;
    case camera::CameraEnum::pei1:
      currCamera = camera::CameraEnum::pei1;
      break;
    default:
      currCamera = camera::CameraEnum::ton1;
  }
}

void Game::setupFourP(const Model* tileModel){
  setupFourPTiles(tileModel);
  setupLockSpaces(game::FourP);
}

void Game::setupThreeP(const Model* tileModel){
  setupThreePTiles(tileModel);
  setupLockSpaces(game::ThreeP);
}


void Game::setupFourPTiles(const Model* tileModel){
  glm::vec3 startPos = glm::vec3(-model::matScale.x / 2.0f + model::tileScale.x / 2.0f,
                                 model::tileScale.y / 2.0f,
                                 -model::matScale.z / 2.0f + model::tileScale.z / 2.0f);

  glm::quat orientation = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  for (int i = tile::Man1; i <= tile::Chun; ++i){
    for (int j = 0; j < 4; ++j){
      int c = (i * 4 + j) % 17;
      int r = (i * 4 + j) / 17;

      glm::vec3 pos = glm::vec3(startPos.x + c * model::tileScale.x, startPos.y, startPos.z + r * model::tileScale.z);
      if (j == 3 && (i == tile::Man5 || i == tile::Sou5 || i == tile::Pin5)){
        if (i == tile::Man5){
          tiles.emplace_back(std::make_unique<Tile>(tile::Man5A, tileModel, pos, orientation, model::tileScale));
        }else if (i == tile::Sou5){
          tiles.emplace_back(std::make_unique<Tile>(tile::Sou5A, tileModel, pos, orientation, model::tileScale));
        }else{
          tiles.emplace_back(std::make_unique<Tile>(tile::Pin5A, tileModel, pos, orientation, model::tileScale));
        }
      }else{
        tiles.emplace_back(std::make_unique<Tile>(i, tileModel, pos, orientation, model::tileScale));
      }
    }
  }
}

void Game::setupThreePTiles(const Model* tileModel){
  glm::vec3 startPos = glm::vec3(-model::matScale.x / 2.0f + model::tileScale.x / 2.0f,
                                 model::tileScale.y / 2.0f,
                                 -model::matScale.z / 2.0f + model::tileScale.z / 2.0f);
  glm::quat orientation = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  for (int i = 0; i < 4; ++i){
      int c = (i) % 18;
      int r = (i) / 18;
      glm::vec3 pos = glm::vec3(startPos.x + c * model::tileScale.x, startPos.y, startPos.z + r * model::tileScale.z);
      tiles.emplace_back(std::make_unique<Tile>(tile::Man1, tileModel, pos, orientation, model::tileScale));
  }
  for (int i = 0; i < 4; ++i){
      int c = (4 + i) % 18;
      int r = (4 + i) / 18;
      glm::vec3 pos = glm::vec3(startPos.x + c * model::tileScale.x, startPos.y, startPos.z + r * model::tileScale.z);
      tiles.emplace_back(std::make_unique<Tile>(tile::Man9, tileModel, pos, orientation, model::tileScale));
  }

  for (int i = tile::Sou1; i <= tile::Chun; ++i){
    for (int j = 0; j < 4; ++j){
      int c = ((i) * 4 + j - 28) % 18;
      int r = ((i) * 4 + j - 28) / 18;

      glm::vec3 pos = glm::vec3(startPos.x + c * model::tileScale.x, startPos.y, startPos.z + r * model::tileScale.z);

      if (j == 3 && (i == tile::Sou5 || i == tile::Pin5)){
        if (i == tile::Sou5){
          tiles.emplace_back(std::make_unique<Tile>(tile::Sou5A, tileModel, pos, orientation, model::tileScale));
        }else{
          tiles.emplace_back(std::make_unique<Tile>(tile::Pin5A, tileModel, pos, orientation, model::tileScale));
        }
      }else{
        tiles.emplace_back(std::make_unique<Tile>(i, tileModel, pos, orientation, model::tileScale));
      }
    }
  }
}

void Game::makeWalls(int tileSet){
  shuffleTiles();

  int tileWidth;
  if (tileSet == tile::TileSet::ThreeP){
    tileWidth = 18;
  }else {
    tileWidth = 17;
  }

  float r = model::wallRadiusDistance;
  float startY = model::tileScale.y / 2.0f;
  float offset = -model::tileScale.x * (tileWidth / 2.0f - 0.5f);

  int i = 0;
  for (std::unique_ptr<Tile>& tile : tiles){
    int x = (i % (tileWidth * 2)) / 2;
    int y = i % 2;
    int z = i / (tileWidth * 2);
    int w = i / 2;

    glm::vec3 pos = glm::vec3(0.0f);
    pos.y = startY + model::tileScale.y * y;

    int xMove = std::cos(glm::radians(90.0f * z));
    int zMove = -std::sin(glm::radians(90.0f * z));
    pos.x = r * xMove; 
    pos.z = r * zMove;

    pos.x += offset * zMove; 
    pos.z += -offset * xMove;

    pos.x += x * model::tileScale.x * zMove; 
    pos.z -= x * model::tileScale.x * xMove;

    tile->position = pos;

    glm::quat orientation = glm::quat(glm::vec3(glm::radians(180.0f), glm::radians(90.0f * (z + 1)), 0.0f));
    tile->orientation = orientation;

    if (y == 0){ 
      yamaLockSpaces[w] = std::make_unique<YamaLockSpace>(pos, orientation, glm::vec3(model::tileScale.x, model::tileScale.y * lock::spaceHeightFactor, model::tileScale.z));
    }
      
    if (yamaLockSpaces[w]){
      yamaLockSpaces[w]->assignTile(tile.get());
    }

    i++;
  }
}

void Game::shuffleTiles(){
  std::random_device rand;
  std::default_random_engine gen(rand());
  std::shuffle(tiles.begin(), tiles.end(), gen);
}

void Game::setup(const game::SetupConfig& config){
  setupConfig(config);
  setupCameras(config);
  
  // setupFourP(config.tileModel);
  setupThreeP(config.tileModel);

  makeWalls(config.tileSet);
  setupDie();
  setupLockSpaces(config.tileSet);
  setupHands(config);
}
