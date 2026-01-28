#include "../include/game/game.hpp"
#include "../include/game/camera.hpp"
#include "../include/game/dice.hpp"
#include "../include/game/tile.hpp"
#include "../include/game/hand.hpp"
#include "../include/engine/input.hpp"
#include "engine/config.hpp"


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
  setupHands(config);
}
