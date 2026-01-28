#ifndef HAND_H
#define HAND_H

#include <glm/glm.hpp>

// namespace hand {
//
// };

class Hand {
public:
  Hand(glm::vec3 positionIn, glm::vec3 colorIn);

  glm::vec3 position;
  glm::vec3 color;

  void update(glm::vec3 rayDir, glm::vec3 rayOrigin);
private:
};

#endif
