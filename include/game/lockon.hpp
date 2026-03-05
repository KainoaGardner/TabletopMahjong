#ifndef LOCKON_H
#define LOCKON_H

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>



namespace lock {
  const float spaceHeightFactor = 0.1f;
}

class Shader;
class Geometry;

class LockSpace {
public:
  LockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn);

  glm::vec3 position;
  glm::quat orientation;
  glm::vec3 scale;

  void draw(Shader* shader, Geometry* geo, const glm::mat4& view, const glm::mat4& projection) const;

  glm::mat4 getModelMatrix() const;

private:
};



#endif
