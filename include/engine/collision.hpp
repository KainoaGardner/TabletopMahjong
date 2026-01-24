#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

namespace collision {
  void computeMouseRay(float mouseX, float mouseY, int width, int height,
                       const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos,
                       glm::vec3& rayOrigin, glm::vec3& rayDir);
}


#endif
