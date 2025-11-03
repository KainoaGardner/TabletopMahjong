#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <glm/glm.hpp>
#include "../include/model.hpp"

namespace camera {
  const glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  const glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  const glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);

  const float yaw = 90.0f;
  const float pitch = -30.0f;
  const float roll = 0.0f;
  const float fov = 45.0f;
  const float speed = 0.01f;
  const float sensitivity = 0.1f;
  const float orthoSensRatio = 0.005f;
  const float zoomSpeed = 1.0f;

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


class Camera {
public:
  Camera(glm::vec3 position, float yaw, float pitch, float roll, float fov, float speed, float sensitivity, float zoomSpeed);

  glm::vec3 position;

  float yaw;
  float pitch;
  float roll;
  
  float fov;

  glm::vec3 front = camera::cameraFront;
  glm::vec3 up = camera::cameraUp;
  glm::vec3 right = camera::cameraRight;


  void update();

  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix() const;

private:
  float startYaw;
  float startPitch;
  float startRoll;

  float startFov;

  float speed;
  float sensitivity;
  float zoomSpeed;

  bool freeCam = false;
  // bool ortho = false;

  void clampPitch();
  void updateVectors();

  void rotate();
  void zoom();
  void move();
};


namespace camera {
enum CameraEnum {
  ton1,
  nan1,
  sha1,
  pei1,
  top,
};

  void setup();
  void switchCamera();

  extern std::unique_ptr<Camera> cameras[5];

  extern int curr;
}


#endif
