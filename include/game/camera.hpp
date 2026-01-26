#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "../include/engine/model.hpp"

namespace camera {
  glm::mat4 getViewMatrix(glm::vec3 position, float yaw, float pitch, float roll);
  glm::mat4 getProjectionMatrix(float fov, float width, float height);

  enum CameraEnum {
    ton1,
    nan1,
    sha1,
    pei1,
    top,
  };

  const glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  const glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  const glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);

  const float yaw = 90.0f;
  const float pitch = -30.0f;
  const float roll = 0.0f;
  const float fov = 45.0f;
  const float speed = 90.0f / 60.0f;
  const float sensitivity = 0.1f;
  const float moveSpeed = 0.001f;
  const float orthoSensRatio = 0.001f;
  const float zoomSpeed = 00005.0f;

  const float orthoZoomRatio = 0.007f;

  // const glm::vec3 perspectivePos = glm::vec3(0.0f, model::matScale.z * 0.4, model::matScale.z * 0.9);
  const glm::vec3 tonPos = glm::vec3(model::matScale.z * 0.9, model::matScale.z * 0.4, 0.0f);
  const glm::vec3 nanPos = glm::vec3(0.0f, model::matScale.z * 0.4, -model::matScale.z * 0.9);
  const glm::vec3 shaPos = glm::vec3(-model::matScale.z * 0.9, model::matScale.z * 0.4, 0.0f);
  const glm::vec3 peiPos = glm::vec3(0.0f, model::matScale.z * 0.4, model::matScale.z * 0.9);

  const glm::vec3 topPos = glm::vec3(0.0f, model::matScale.z * 1.3f, 0.0f);

  const glm::vec3 orthoPos = glm::vec3(0.0f,0.5f,0.5f);

  const float perspectivePitch = -30.0f;
  const float orthoPitch = -45.0f;

  const float minFov = 20.0f;
  const float maxFov = 45.0f;
} 

class Input;

class Camera {
public:
  Camera(glm::vec3 position, float yaw, float pitch, float roll, float fov, float speed, float moveSpeed, float sensitivity, float zoomSpeed);

  glm::vec3 position;

  float yaw;
  float pitch;
  float roll;
  
  float fov;

  glm::vec3 front = camera::cameraFront;
  glm::vec3 up = camera::cameraUp;
  glm::vec3 right = camera::cameraRight;

  void update(const Input& input, bool topCamera = false);
  void gameUpdate(const Input& input, bool topCamera = false);

  void revert(bool topCamera = false);

  // glm::mat4 getViewMatrix() const;
  // glm::mat4 getProjectionMatrix() const;

private:
  float startYaw;
  float startPitch;
  float startRoll;

  float startFov;

  float speed;
  float sensitivity;
  float moveSpeed;
  float zoomSpeed;

  bool freeCam = false;

  void clampAngles();
  void clampPosition();

  void updateVectors();

  void rotate(const Input& input);
  void revertRotate(const Input& input);
  void revertPosition(const Input& input);
  void zoom(const Input& input);
  void move(const Input& input);
};


#endif
