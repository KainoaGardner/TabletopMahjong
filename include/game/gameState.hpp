#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace gameState {

struct CameraState {
  glm::vec3 position;
  float yaw;
  float pitch;
  float roll;
  float fov;
};

struct TileState {
  glm::vec3 position;
  glm::quat orientation;
};

struct DiceState {
  glm::vec3 position;
  glm::quat orientation;
};

class GameState {
public:
  bool firstFrame = true;

  std::vector<TileState> tiles;
  std::vector<DiceState> die;
  CameraState cameras[5];
private:
};
}

#endif
