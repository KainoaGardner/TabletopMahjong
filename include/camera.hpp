#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <glm/glm.hpp>
#include "../include/model.hpp"

namespace camera {
  const glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  const glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  const glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);

  const float yaw = 0.0f;
  const float pitch = -30.0f;
  const float roll = 0.0f;
  const float fov = 45.0f;
  const float speed = 0.01f;
  const float sensitivity = 0.1f;
  const float orthoSensRatio = 0.005f;
  const float zoomSpeed = 1.0f;

  const float orthoZoomRatio = 0.006f;

  const glm::vec3 perspectivePos = glm::vec3(0.0f,0.4f,model::matScale.z * 0.7);
  const glm::vec3 orthoPos = glm::vec3(0.0f,0.3f,0.3f);

  const float perspectivePitch = -40.0f;
  const float orthoPitch = -45.0f;

  const float minFov = 20.0f;
  const float maxFov = 45.0f;
} 


class Camera {
public:
  Camera(glm::vec3 position, float yaw, float pitch, float roll, float fov, float speed, float sensitivity, float zoomSpeed);

  glm::vec3 position;

  float yaw ;
  float pitch;
  float roll;

  float fov;

  float speed;
  float sensitivity;
  float zoomSpeed;

  bool ortho = false;

  glm::vec3 front = camera::cameraFront;
  glm::vec3 up = camera::cameraUp;
  glm::vec3 right = camera::cameraRight;

  // void update();

  void rotate();
  void zoom();
  void move();

  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix() const;

private:
  void clampPitch();
  void updateVectors();
};


namespace camera {
  struct Cameras {
    std::unique_ptr<Camera> normal;
  };

  void setup();
  extern Cameras cameras;
}


#endif
