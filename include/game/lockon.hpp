#ifndef LOCKON_H
#define LOCKON_H

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace lock {
  const float spaceHeightFactor = 0.1f;
  const float borderThickness = 0.01f;
}

class Shader;
class Geometry;

class Tile;

class LockSpace {
public:
  LockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn);

  glm::vec3 position;
  glm::quat orientation;
  glm::vec3 scale;

  Tile* tile;
  bool hovered = false;

  void draw(Shader* shader, Geometry* geo, const glm::mat4& view, const glm::mat4& projection) const;
  virtual void assignTile(Tile* assignTile);
  glm::mat4 getModelMatrix() const;

private:
};

  // std::unique_ptr<LockSpace> handLockSpaces[global::handLockSpaceAmount];
  // std::unique_ptr<LockSpace> discardLockSpaces[global::discardLockSpaceAmount];
  // std::unique_ptr<LockSpace> callLockSpaces[global::callLockSpaceAmount];
  // std::unique_ptr<LockSpace> yamaLockSpaces[global::yamaLockSpaceAmount];


class HandLockSpace : public LockSpace {
public:
  HandLockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn);

  void assignTile(Tile* assignTile) override;
private:
};

class DiscardLockSpace : public LockSpace {
public:
  DiscardLockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn);

  void assignTile(Tile* assignTile) override;
private:
};

class CallLockSpace : public LockSpace {
public:
  CallLockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn);

  void assignTile(Tile* assignTile) override;
private:
};

class YamaLockSpace : public LockSpace {
public:
  YamaLockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn);

  Tile* tile[2];

  void assignTile(Tile* assignTile) override;
private:
};

#endif
