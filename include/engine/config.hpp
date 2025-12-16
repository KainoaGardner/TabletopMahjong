#ifndef CONFIG_H
#define CONFIG_H

#include <limits>
#include <glm/glm.hpp>

namespace global {
  const float maxCounter = std::numeric_limits<float>::max();

  const glm::vec3 worldFront = glm::vec3(0.0f, 0.0f, 1.0f);
  const glm::vec3 worldUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  const glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);

  const float nearPlane = 0.1f;
  const float farPlane = 10000.0f;

  const int maxTimeGap = 250;
} // namespace global

#endif
