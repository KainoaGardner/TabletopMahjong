#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <glm/glm.hpp>



namespace camera {
  const glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  const glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  const glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);

  const float yaw = 0.0f;
  const float pitch = -35.0f;
  const float roll = 0.0f;
  const float fov = 45.0f;
  const float speed = 0.01f;
  const float sensitivity = 0.1f;
  const float zoomSpeed = 1.0f;

  const float minFov = 15.0f;
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
