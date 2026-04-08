#include "game/game.hpp"
#include "engine/model.hpp"
#include "game/camera.hpp"
#include "game/dice.hpp"
#include "game/tile.hpp"
#include "game/hand.hpp"
#include "game/lockon.hpp"

#include "engine/input.hpp"
#include "engine/config.hpp"


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
  setupHandLockSpaces(tileSet);
  setupDiscardLockSpaces(tileSet);
  setupCallLockSpaces(tileSet);
  setupYamaLockSpaces(tileSet);
}

void Game::setupHandLockSpaces(int tileSet){
  int walls;
  if (tileSet == tile::TileSet::ThreeP){
    walls = 3;
  }else {
    walls = 4;
  }

  float r = model::handRadiusDistance;
  float startY = 0.0;
  float offset = -model::tileScale.x * (13.0f / 2.0f - 0.5f);

  for (int i = 0; i < 13 * walls; i++){
    int x = i % 13;
    int z = (i / 13); 

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

    // glm::quat o1 = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat orientation = glm::angleAxis(glm::radians(90.0f * (z + 1)), glm::vec3(0.0f, 1.0f, 0.0f));
    handLockSpaces[i] = std::make_unique<LockSpace>(pos, orientation, glm::vec3(model::tileScale.x, model::tileScale.y * lock::spaceHeightFactor, model::tileScale.z));
  }

  for (int i = 0; i < walls; i++){
    float x = 13.5f;
    int z = i; 

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
    handLockSpaces[52 + z] = std::make_unique<LockSpace>(pos, orientation, glm::vec3(model::tileScale.x, model::tileScale.y * lock::spaceHeightFactor, model::tileScale.z));
  }
}

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
    discardLockSpaces[i] = std::make_unique<LockSpace>(pos, orientation, glm::vec3(model::tileScale.x, model::tileScale.y * lock::spaceHeightFactor, model::tileScale.z));
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
    callLockSpaces[i] = std::make_unique<LockSpace>(pos, orientation, glm::vec3(model::tileScale.x, model::tileScale.y * lock::spaceHeightFactor, model::tileScale.z));
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
    callLockSpaces[16 * 4 + i] = std::make_unique<LockSpace>(pos, orientation, glm::vec3(model::tileScale.x, model::tileScale.y * lock::spaceHeightFactor, model::tileScale.z));
  }
}

void Game::setupYamaLockSpaces(int tileSet){
  int tileWidth;
  int wallCount;
  if (tileSet == tile::TileSet::ThreeP){
    tileWidth = 18;
    wallCount = 3;
  }else {
    tileWidth = 17;
    wallCount = 4;
  }

  float r = model::wallRadiusDistance;
  float startY = 0.0;
  float offset = -model::tileScale.x * (tileWidth / 2.0f - 0.5f);

  for (int i = 0; i < tileWidth * wallCount; i++){
    int x = i % (tileWidth);
    int z = i / (tileWidth);

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
    yamaLockSpaces[i] = std::make_unique<YamaLockSpace>(pos, orientation, glm::vec3(model::tileScale.x, model::tileScale.y * lock::spaceHeightFactor, model::tileScale.z));
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

void Game::setup(const game::SetupConfig& config){
  setupConfig(config);
  setupCameras(config);
  tile::setup(config, tiles);
  setupDie();
  setupLockSpaces(config.tileSet);
  setupHands(config);
}
