#include "../include/camera.hpp"
#include "../include/config.hpp"

#include <glm/gtc/quaternion.hpp>

Camera::Camera(
  glm::vec3 positionIn,
  float yawIn,
  float pitchIn,
  float rollIn,
  float fovIn,
  float speedIn,
  float sensitivityIn){

  position = positionIn;
  yaw = yawIn;
  pitch = pitchIn;
  roll = rollIn;
  fov = fovIn;
  speed = speedIn;
  sensitivity = sensitivityIn;

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

  clampPitch();
  updateVectors();
}

void Camera::zoom(){
}

void Camera::move(){
}


namespace camera {
  Cameras cameras;

  void setup(){
    glm::vec3 position = glm::vec3(0.0f,0.0f,3.0f);
    cameras.normal = std::make_unique<Camera>(position,
                                              0.0f,
                                              0.0f,
                                              0.0f,
                                              45.0f,
                                              0.01f,
                                              0.1f);
  }
}
