#include "../include/game/camera.hpp"
#include "../include/engine/config.hpp"
#include "../include/engine/input.hpp"

#include <glm/gtc/quaternion.hpp>

#include "../include/engine/model.hpp"


Camera::Camera(
  glm::vec3 positionIn,
  float yawIn,
  float pitchIn,
  float rollIn,
  float fovIn,
  float speedIn,
  float moveSpeedIn,
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
  moveSpeed = moveSpeedIn;
  sensitivity = sensitivityIn;
  zoomSpeed = zoomSpeedIn;

  clampAngles();
  updateVectors();
}

void Camera::clampAngles() {
  float pitchDif = pitch - startPitch;
  float yawDif = yaw - startYaw;

  if (pitch > 89.9f) {
    pitch = 89.9f; 
  }

  if (pitch < -89.9f) { 
    pitch = -89.9f; 
  }

  if (yawDif > 89.9f) {
    yaw = startYaw + 89.9f; 
  }

  if (yawDif < -89.9f) { 
    yaw = startYaw - 89.9f; 
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

void Camera::rotate(const Input& input){
  if (input.pressed(input::actions::freeLook)){
    yaw -= input.mouse.dx   * sensitivity;
    pitch -= input.mouse.dy * sensitivity;

  }else {
    // pitch = startPitch;
    // yaw = startYaw;
  }

  clampAngles();
  updateVectors();
}

void Camera::revertRotate(const Input& input){
  if (input.pressed(input::actions::freeLook)) return; 

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

void Camera::revertPosition(const Input& input){
  if (input.pressed(input::actions::freeLook)) return; 

  float xDiff = position.x;
  float zDiff = position.z;

  float xDist = std::abs(xDiff);
  float zDist = std::abs(zDiff);

  float maxDist = std::max(xDist, zDist);

  if (maxDist < 0.001f) {
    position.x = 0.0f;
    position.z = 0.0f;
    return;
  }

  float returnSpeed = moveSpeed * 5.0f;

  float xSpeed = returnSpeed * (xDist / maxDist);
  float zSpeed = returnSpeed * (zDist / maxDist);

  if (xDiff >= xSpeed){
    position.x -= xSpeed;
  }else if (xDiff <= -xSpeed){
    position.x += xSpeed;
  }else{
    position.x = 0.0f;
  }

  if (zDiff >= zSpeed){
    position.z -= zSpeed;
  }else if (zDiff <= -zSpeed){
    position.z += zSpeed;
  }else{
    position.z = 0.0f;
  }
}

void Camera::zoom(const Input& input){
  if (input.pressed(input::actions::zoomIn)){
    fov += zoomSpeed;
  }
  if (input.pressed(input::actions::zoomOut)){
    fov -= zoomSpeed;
  }

  if (fov < camera::minFov){
    fov = camera::minFov;
  }
  if (fov > camera::maxFov){
    fov = camera::maxFov;
  }
}

void Camera::move(const Input& input){
  if (input.pressed(input::actions::freeLook)){
    position.z += input.mouse.dx * moveSpeed;
    position.x -= input.mouse.dy * moveSpeed;
  }else {
    // pitch = startPitch;
    // yaw = startYaw;
  }

  clampPosition();
  updateVectors();
}

void Camera::clampPosition(){
  // float clampPos = model::matScale
  float clampPos = model::matScale.z / 2.0f - 1.5f * model::tileScale.z;

  if (position.z > clampPos){
    position.z = clampPos;
  }else if (position.z < -clampPos){
    position.z = -clampPos;
  }

  if (position.x > clampPos){
    position.x = clampPos;
  }else if (position.x < -clampPos){
    position.x = -clampPos;
  }


}

void Camera::update(const Input& input, bool topCamera){
  if (!topCamera){
    rotate(input);
  }else {
    move(input);
  }

  zoom(input);

  if (input.justPressed(input::actions::revertCamera)){
    revert(topCamera);
  }
}

void Camera::revert(bool topCamera){
    fov = startFov;

    if (!topCamera){
      yaw = startYaw;
      pitch = startPitch;
    }else{
      position.x = 0.0f;
      position.z = 0.0f;
    }
}

void Camera::gameUpdate(const Input& input, bool topCamera){
  // if (!topCamera){
  //   revertRotate(input);
  // }else{
  //   revertPosition(input);
  // }
}

namespace camera {

glm::mat4 getViewMatrix(glm::vec3 position, float yaw, float pitch, float roll) {
  const float yawR = glm::radians(yaw);
  const float pitchR = glm::radians(pitch);
  const float rollR = glm::radians(roll);

  glm::quat qYaw = glm::angleAxis(yawR,  global::worldUp);
  glm::quat qPitch = glm::angleAxis(pitchR,global::worldRight);
  glm::quat qRoll = glm::angleAxis(rollR, global::worldFront);

  glm::quat orientation = qYaw * qPitch * qRoll;
  orientation = glm::normalize(orientation);

  glm::vec3 front = glm::normalize(orientation * camera::cameraFront);
  glm::vec3 up = glm::normalize(orientation * camera::cameraUp);

  return glm::lookAt(position,position + front, up);
}

glm::mat4 getProjectionMatrix(float fov, float width, float height) {
  return glm::perspective(
    glm::radians(fov),
    width / height,
    global::nearPlane,
    global::farPlane);
}
}
