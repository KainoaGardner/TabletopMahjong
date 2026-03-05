#include "engine/shader.hpp"
#include "engine/geometry.hpp"

#include "game/lockon.hpp"


namespace lock {

}

LockSpace::LockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn){
  position = positionIn;
  orientation = orientationIn;
  scale = scaleIn;
}

glm::mat4 LockSpace::getModelMatrix() const{
  glm::mat4 model = glm::mat4(1.0f);

  model = glm::translate(model, position);
  model *= glm::mat4_cast(orientation);
  model = glm::scale(model, glm::vec3(scale.x, scale.y * lock::spaceHeightFactor, scale.z));

  return model;
}

void LockSpace::draw(Shader* shader, Geometry* geo, const glm::mat4& view, const glm::mat4& projection) const {
  shader->use();

  glBindVertexArray(geo->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geo->ebo);

  shader->setMatrix4fv("uView", view);
  shader->setMatrix4fv("uProjection", projection);
  shader->setVec4f("uColor", glm::vec4(glm::vec3(0.6f), 0.75f));
  glm::mat4 model = getModelMatrix();

  shader->setMatrix4fv("uModel", model);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

