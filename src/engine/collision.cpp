#include "../include/engine/collision.hpp"
#include "../include/game/tile.hpp"
#include "../include/engine/model.hpp"
#include "engine/model.hpp"

#include <iostream>

namespace collision {
  void computeMouseRay(float mouseX, float mouseY, int width, int height,
                       const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos,
                       glm::vec3& rayOrigin, glm::vec3& rayDir){

  float x = (2.0f * mouseX) / float(width) - 1.0f;
  float y = 1.0f - (2.0f * mouseY) / float(height);
  glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

  glm::vec4 rayCamera = glm::inverse(projection) * rayClip;
  rayCamera.z = -1.0f;
  rayCamera.w = 0.0f;

  rayDir = glm::normalize(glm::vec3(glm::inverse(view) * rayCamera));
  rayOrigin = cameraPos;
}

AABB convertTileToAABB(const std::unique_ptr<Tile>& tile){
  AABB result;

  result.min = -tile->halfSize;
  result.max = tile->halfSize;
  return result;
}

bool rayIntersectAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const AABB& box, float& tHit){
  glm::vec3 invDir = 1.0f / rayDir;

  glm::vec3 t0 = (box.min - rayOrigin) * invDir;
  glm::vec3 t1 = (box.max - rayOrigin) * invDir;

  glm::vec3 tMin = glm::min(t0, t1);
  glm::vec3 tMax = glm::max(t0, t1);

  float tNear = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
  float tFar = glm::min(glm::min(tMax.x, tMax.y), tMax.z);

  if (tNear > tFar || tFar < 0.0f){
    return false;
  }

  tHit = tNear;
  return true;
}


Tile* pickTile(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const std::vector<std::unique_ptr<Tile>>& tiles){
  Tile* result = nullptr;
  float closestDist = FLT_MAX;

  for (const auto& tile : tiles){
    AABB hitbox = convertTileToAABB(tile);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, tile->position);
    model *= glm::mat4_cast(tile->orientation);
    model = glm::scale(model, glm::vec3(model::tileScaleFactor));

    glm::mat4 invModel = glm::inverse(model);

    glm::vec3 localOrigin = glm::vec3(invModel * glm::vec4(rayOrigin, 1.0f));
    glm::vec3 localDir = glm::normalize(glm::vec3(invModel * glm::vec4(rayDir, 0.0f)));

    float t;
    if (rayIntersectAABB(localOrigin, localDir, hitbox, t)){
      if (t < closestDist){
        closestDist = t;
        result = tile.get();
      }
    }
  }

  return result;
}

}

