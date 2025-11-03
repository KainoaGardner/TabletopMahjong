#include "../include/camera.hpp"
#include "../include/config.hpp"
#include "../include/input.hpp"
#include "../include/game.hpp"

#include <glm/gtc/quaternion.hpp>
#include <memory>

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
  // if (ortho){
  //   float aspect = (float)config::gameConfig.width / (float)config::gameConfig.height;
  //   float zoom = fov * camera::orthoZoomRatio;
  //
  //   return glm::ortho(
  //     -zoom * aspect,
  //     zoom * aspect,
  //     -zoom,
  //     zoom,
  //     config::gameConfig.nearPlane,
  //     config::gameConfig.farPlane);
  //
  // }else {
    return glm::perspective(
      glm::radians(fov),
      (float)config::gameConfig.width / (float)config::gameConfig.height,
      config::gameConfig.nearPlane,
      config::gameConfig.farPlane);
  // }
}


void Camera::clampPitch() {
  if (pitch > 89.9f) { pitch = 89.9f; }
  if (pitch < -89.9f) { pitch = -89.9f; }
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
    pitch = startPitch;
    yaw = startYaw;
    // fov = startFov;
  }

  clampPitch();
  updateVectors();
}

void Camera::zoom(){
  // if (!input::actionPressed[input::freeLook]) return;

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
  // if (input::actionPressed[input::freeCam]){
  //   freeCam = !freeCam;
  //   input::actionPressed[input::freeCam] = false;
  // }
  //
  // if (input::actionPressed[input::forward]){
  //   position += speed * front;
  // }
  //
  // if (input::actionPressed[input::backward]){
  //   position -= speed * front;
  // }
  //
  // if (input::actionPressed[input::right]){
  //   position += speed * right;
  // }
  //
  // if (input::actionPressed[input::left]){
  //   position -= speed * right;
  // }
  //
  // if (input::actionPressed[input::up]){
  //   position += speed * global::worldUp;
  // }
  //
  // if (input::actionPressed[input::down]){
  //   position -= speed * global::worldUp;
  // }

  // if (input::actionPressed[input::perspective]){
  //   ortho = !ortho;
  //   if (ortho){
  //     position = camera::orthoPos;
  //     pitch = camera::orthoPitch;
  //   }else{
  //     position = camera::perspectivePos;
  //     pitch = camera::perspectivePitch;
  //   }
  //
  //   fov = camera::fov;
  //   yaw = camera::yaw;
  //
  //   input::actionPressed[input::perspective] = false;
  // }
  //
  // if (ortho){
  //   if (input::actionPressed[input::freeLook]){
  //     position.x -= input::mouse.dx * sensitivity * camera::orthoSensRatio;
  //     position.z -= input::mouse.dy * sensitivity * camera::orthoSensRatio;
  //
  //   }else {
  //     position = camera::orthoPos;
  //   }
  // }else {
  //   if (!freeCam){
  //     position = camera::perspectivePos;
  //   }
  // }
  

  // position = startPosition;
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
