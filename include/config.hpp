#ifndef CONFIG_H
#define CONFIG_H

#include <limits>

namespace global {
  const float maxCounter = std::numeric_limits<float>::max();
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

};

// struct DisplayContext {
//   glm::mat4 projection;
//   glm::mat4 view;
//   glm::mat4 textProjection;
// };


extern GameConfig gameConfig;
} // namespace config

#endif
