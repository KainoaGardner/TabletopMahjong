#include "../include/game/game.hpp"
#include "../include/game/camera.hpp"
#include "../include/game/dice.hpp"
#include "../include/game/tile.hpp"
#include "../include/engine/input.hpp"

Game::Game(){}

Game::~Game() = default;

void Game::update(const Input& input){
  switchCamera(input);
}

void Game::switchCamera(const Input& input){
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
}

void Game::setupCameras(){
  cameras[0] = std::make_unique<Camera>(
    camera::tonPos, camera::yaw, camera::pitch, camera::roll, camera::fov, camera::speed, camera::sensitivity, camera::zoomSpeed);

  cameras[1] = std::make_unique<Camera>(
    camera::nanPos, camera::yaw + 90.0f, camera::pitch, camera::roll, camera::fov, camera::speed, camera::sensitivity, camera::zoomSpeed);

  cameras[2] = std::make_unique<Camera>(
    camera::shaPos, camera::yaw + 180.0f, camera::pitch, camera::roll, camera::fov, camera::speed, camera::sensitivity, camera::zoomSpeed);

  cameras[3] = std::make_unique<Camera>(
    camera::peiPos, camera::yaw + 270.0f, camera::pitch, camera::roll, camera::fov, camera::speed, camera::sensitivity, camera::zoomSpeed);

  cameras[4] = std::make_unique<Camera>(
    camera::topPos, camera::yaw, -90.0f, camera::roll, camera::fov, camera::speed, camera::sensitivity, camera::zoomSpeed);

}

void Game::setupDie(){
  die[0] = std::make_unique<Dice>();
  die[1] = std::make_unique<Dice>();

  // int roll = dice::getDiceRoll() + dice::getDiceRoll();
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
  setupCameras();
  tile::setup(config, tiles);
  setupDie();
}
