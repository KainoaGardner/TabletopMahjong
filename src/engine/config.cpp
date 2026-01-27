#include "../include/engine/config.hpp"

namespace global {
std::unordered_map<colors, glm::vec3> colorToVec3 = {
    { red,    glm::vec3(1.0f, 0.0f, 0.0f) },
    { orange, glm::vec3(1.0f, 0.5f, 0.0f) },
    { yellow, glm::vec3(1.0f, 1.0f, 0.0f) },
    { green,  glm::vec3(0.0f, 1.0f, 0.0f) },
    { blue,   glm::vec3(0.0f, 0.0f, 1.0f) },
    { purple, glm::vec3(0.5f, 0.0f, 0.5f) },
    { white,  glm::vec3(1.0f, 1.0f, 1.0f) },
    { gray,   glm::vec3(0.5f, 0.5f, 0.5f) },
    { black,  glm::vec3(0.0f, 0.0f, 0.0f) },
    { brown,  glm::vec3(0.6f, 0.3f, 0.1f) } 
};

}
