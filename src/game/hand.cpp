#include "../include/game/hand.hpp"

Hand::Hand(glm::vec3 positionIn, glm::vec3 colorIn){
  position = positionIn;
  color = colorIn;
}

void Hand::update(glm::vec3 rayDir, glm::vec3 rayOrigin){
  float t = -rayOrigin.y / rayDir.y;
  position = rayOrigin + t * rayDir;
}
