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

YamaLockSpace::YamaLockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn) : LockSpace(positionIn, orientationIn, scaleIn){
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

void LockSpace::assignTile(Tile* assignTile){
  if (tile == nullptr){
    tile = assignTile;
  }
}

void YamaLockSpace::assignTile(Tile* assignTile){
  if (tiles[0] == nullptr){
    tiles[0] = assignTile;
  }else if (tiles[1] == nullptr){
    tiles[1] = assignTile;
  }
}

void LockSpace::draw(Shader* shader, Geometry* geo, const glm::mat4& view, const glm::mat4& projection) const {
  shader->use();

  glBindVertexArray(geo->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geo->ebo);

  shader->setMatrix4fv("uView", view);
  shader->setMatrix4fv("uProjection", projection);


  glm::vec4 color = glm::vec4(glm::vec3(0.6f), 0.75f);
  glm::vec4 borderColor = glm::vec4(glm::vec3(1.0f), 0.75f);
  if (hovered){
    color = color * glm::vec4(glm::vec3(0.5f), 1.0f);
    borderColor.w = 1.0f;
  }

  shader->setVec4f("uColor", color);
  shader->setVec4f("uBorderColor", borderColor);

  glm::mat4 model = getModelMatrix();

  shader->setMatrix4fv("uModel", model);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

