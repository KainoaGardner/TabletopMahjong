#include "../include/camera.hpp"
#include "../include/config.hpp"
#include "../include/input.hpp"
#include "../include/game.hpp"

#include <glm/gtc/quaternion.hpp>
#include <memory>


#include <iostream>

Camera::Camera(
  glm::vec3 positionIn,
  float yawIn,
  float pitchIn,
  float rollIn,
  float fovIn,
  float speedIn,
  float sensitivityIn,
  float zoomSpeedIn){

  position = positionIn;
  yaw = yawIn;
  startYaw = yawIn;
  pitch = pitchIn;
  startPitch = pitchIn;
  roll = rollIn;
  startRoll = rollIn;
  fov = fovIn;
  startFov = fovIn;
  speed = speedIn;
  sensitivity = sensitivityIn;
  zoomSpeed = zoomSpeedIn;

  clampPitch();
  updateVectors();
}

glm::mat4 Camera::getViewMatrix() const {
  return glm::lookAt(position,position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
  return glm::perspective(
    glm::radians(fov),
    (float)config::gameConfig.width / (float)config::gameConfig.height,
    config::gameConfig.nearPlane,
    config::gameConfig.farPlane);
}


void Camera::clampPitch() {
  float pitchDif = pitch - startPitch;

  if (pitchDif > 89.9f) {
    pitch = startPitch + 89.9f; 
  }

  if (pitchDif < -89.9f) { 
    pitch = startPitch - 89.9f; 
  }
}

void Camera::updateVectors() {
  const float yawR = glm::radians(yaw);
  const float pitchR = glm::radians(pitch);
  const float rollR = glm::radians(roll);

  glm::quat qYaw = glm::angleAxis(yawR,  global::worldUp);
  glm::quat qPitch = glm::angleAxis(pitchR,global::worldRight);
  glm::quat qRoll = glm::angleAxis(rollR, global::worldFront);

  glm::quat orientation = qYaw * qPitch * qRoll;
  orientation = glm::normalize(orientation);

  front = glm::normalize(orientation * camera::cameraFront);
  up = glm::normalize(orientation * camera::cameraUp);
  right = glm::normalize(orientation * camera::cameraRight);
}

void Camera::rotate(){
  if (input::actionPressed[input::freeLook]){
    yaw -= input::mouse.dx * sensitivity;
    pitch -= input::mouse.dy * sensitivity;
  }else {
    // pitch = startPitch;
    // yaw = startYaw;
  }

  clampPitch();
  revertRotate();
  updateVectors();
}

void Camera::revertRotate(){
  if (input::actionPressed[input::freeLook]) return; 


  float yawDiff = yaw - startYaw;
  float pitchDiff = pitch - startPitch;

  float yawDist = std::abs(yawDiff);
  float pitchDist = std::abs(pitchDiff);

  float maxDist = std::max(yawDist, pitchDist);

  if (maxDist < 0.001f) {
    yaw = startYaw;
    pitch = startPitch;
    return;
  }

  float yawSpeed = speed * (yawDist / maxDist);
  float pitchSpeed = speed * (pitchDist / maxDist);

  if (yawDiff >= yawSpeed){
    yaw -= yawSpeed;
  }else if (yawDiff <= -yawSpeed){
    yaw += yawSpeed;
  }else{
    yaw = startYaw;
  }

  if (pitchDiff >= pitchSpeed){
    pitch -= pitchSpeed;
  }else if (pitchDiff <= -pitchSpeed){
    pitch += pitchSpeed;
  }else{
    pitch = startPitch;
  }
}

void Camera::zoom(){
  if (input::actionPressed[input::zoomIn]){
    fov += zoomSpeed;
  }
  if (input::actionPressed[input::zoomOut]){
    fov -= zoomSpeed;
  }

  if (fov < camera::minFov){
    fov = camera::minFov;
  }
  if (fov > camera::maxFov){
    fov = camera::maxFov;
  }
}

void Camera::move(){
}

void Camera::update(){
  rotate();
  zoom();
}

namespace camera {
  std::unique_ptr<Camera> cameras[5] = {
    std::make_unique<Camera>(tonPos, yaw, pitch, roll, fov, speed, sensitivity, zoomSpeed),
    std::make_unique<Camera>(nanPos, yaw + 90.0f, pitch, roll, fov, speed, sensitivity, zoomSpeed),
    std::make_unique<Camera>(shaPos, yaw + 180.0f, pitch, roll, fov, speed, sensitivity, zoomSpeed),
    std::make_unique<Camera>(peiPos, yaw + 270.0f, pitch, roll, fov, speed, sensitivity, zoomSpeed),
    std::make_unique<Camera>(topPos, yaw, -90.0f, roll, fov, speed, sensitivity, zoomSpeed),
};

int curr = 0;
void setup(){
}

void switchCamera(){
  if (input::actionPressed[input::mainCamera]){
    curr = game::game->seat;
    input::actionPressed[input::mainCamera] = false;
  }else if (input::actionPressed[input::pointCamera]){
    curr = game::game->seat;
    input::actionPressed[input::pointCamera] = false;
  }else if (input::actionPressed[input::topCamera]){
    curr = 4;
    input::actionPressed[input::topCamera] = false;
  }

  //remove
  if (input::actionPressed[input::camera1]){
    curr = 0;
    input::actionPressed[input::camera1] = false;
  }else if (input::actionPressed[input::camera2]){
    curr = 1;
    input::actionPressed[input::camera2] = false;
  }else if (input::actionPressed[input::camera3]){
    curr = 2;
    input::actionPressed[input::camera3] = false;
  }else if (input::actionPressed[input::camera4]){
    curr = 3;
    input::actionPressed[input::camera4] = false;
  }


}


}
