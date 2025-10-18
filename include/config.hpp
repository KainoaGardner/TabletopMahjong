#ifndef CONFIG_H
#define CONFIG_H

#include <limits>
#include <glm/glm.hpp>

namespace global {
  const float maxCounter = std::numeric_limits<float>::max();

  const glm::vec3 worldFront = glm::vec3(0.0f, 0.0f, 1.0f);
  const glm::vec3 worldUp    = glm::vec3(0.0f, 1.0f, 0.0f);
  const glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
} // namespace global

namespace config {
struct GameConfig {
  const unsigned int fps = 60.0;
  const float logicIntervalTime = 1.0f / fps;
  long long lastUpdateTime = 0;
  // unsigned int width = 1280;
  // unsigned int height = 720;
  unsigned int width = 960;
  unsigned int height = 540;

  const float nearPlane = 0.1f;
  const float farPlane = 10000.0f;
};

// struct DisplayContext {
//   glm::mat4 projection;
//   glm::mat4 view;
//   glm::mat4 textProjection;
// };


extern GameConfig gameConfig;
} // namespace config

#endif
