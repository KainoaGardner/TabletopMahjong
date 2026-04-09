#include "engine/shader.hpp"
#include "engine/geometry.hpp"

#include "game/lockon.hpp"
#include "game/tile.hpp"


#include <iostream>

namespace lock {

}

LockSpace::LockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn){
  position = positionIn;
  orientation = orientationIn;
  scale = scaleIn;
}

HandLockSpace::HandLockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn) : LockSpace(positionIn, orientationIn, scaleIn){ }
DiscardLockSpace::DiscardLockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn) : LockSpace(positionIn, orientationIn, scaleIn){ }
CallLockSpace::CallLockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn) : LockSpace(positionIn, orientationIn, scaleIn){ }
YamaLockSpace::YamaLockSpace(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn) : LockSpace(positionIn, orientationIn, scaleIn){ }

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

  assignTile->position = position;
}

void HandLockSpace::assignTile(Tile* assignTile){
  LockSpace::assignTile(assignTile);
  std::cout << "hand" << std::endl;
}

void DiscardLockSpace::assignTile(Tile* assignTile){
  LockSpace::assignTile(assignTile);
  std::cout << "discard" << std::endl;
}

void CallLockSpace::assignTile(Tile* assignTile){
  LockSpace::assignTile(assignTile);
  std::cout << "call" << std::endl;
}

void YamaLockSpace::assignTile(Tile* assignTile){
  if (tile[0] == nullptr){
    tile[0] = assignTile;
  }else if (tile[1] == nullptr){
    tile[1] = assignTile;
  }

  std::cout << "yama" << std::endl;
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

