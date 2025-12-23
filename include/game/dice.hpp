#ifndef DICE_H
#define DICE_H

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class Dice {
public:
  glm::vec3 position;
  glm::quat orientation;

  int getDiceRoll();

private:
};

#endif
