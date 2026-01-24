#include "../include/engine/collision.hpp"

namespace collision {
  void computeMouseRay(float mouseX, float mouseY, int width, int height,
                       const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos,
                       glm::vec3& rayOrigin, glm::vec3& rayDir){

  float x = (2.0f * mouseX) / float(width) - 1.0f;
  float y = 1.0f - (2.0f * mouseY) / float(height);
  glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

  glm::vec4 rayCamera = glm::inverse(projection) * rayClip;
  rayCamera.z = -1.0f;
  rayCamera.w = 0.0f;

  rayDir = glm::normalize(glm::vec3(glm::inverse(view) * rayCamera));
  rayOrigin = cameraPos;
}

}

