#ifndef CONFIG_H
#define CONFIG_H

#include <limits>
#include <glm/glm.hpp>
#include <unordered_map>

namespace global {

const float maxCounter = std::numeric_limits<float>::max();

const glm::vec3 worldFront = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 worldUp    = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);

const float nearPlane = 0.1f;
const float farPlane = 100.0f;

const int maxTimeGap = 250;

const float dragDistThreshold = 8.0f;
const float dragTimeThreshold = 300.0f;

const glm::vec2 selectionBoxThickness = glm::vec2(5.0f);


const int diceAmount = 2;
const int cameraAmount = 5;
const int handAmount = 4;

const int handLockSpaceAmount = 56;
const int discardLockSpaceAmount = 96;
const int callLockSpaceAmount = 80;


enum colors{
  red,
  orange,
  yellow,
  green,
  blue,
  purple,
  white,
  gray,
  black,
  brown
};

enum players{
  jicha,
  shimocha,
  toimen,
  kamicha,
};


  extern std::unordered_map<colors, glm::vec3> colorToVec3;

} // namespace global

#endif
