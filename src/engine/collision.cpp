#include "../include/engine/collision.hpp"
#include "../include/game/tile.hpp"
#include "../include/engine/model.hpp"
#include "engine/model.hpp"

#include <iostream>

namespace collision {
  void computeMouseRay(float mouseX, float mouseY, int width, int height,
                       const glm::mat4& inverseView, const glm::mat4& inverseProjection, const glm::vec3& cameraPos,
                       glm::vec3& rayOrigin, glm::vec3& rayDir){

  float x = (2.0f * mouseX) / float(width) - 1.0f;
  float y = 1.0f - (2.0f * mouseY) / float(height);
  glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

  glm::vec4 rayCamera = inverseProjection * rayClip;
  rayCamera.z = -1.0f;
  rayCamera.w = 0.0f;

  rayDir = glm::normalize(inverseView * rayCamera);
  rayOrigin = cameraPos;
}

void convertV2toFrustumPoints(const glm::vec2& pos, int width, int height, const glm::mat4& inverseView, const glm::mat4& inverseProjection,
                                 glm::vec3& nearPoint, glm::vec3& farPoint){
  float x = (2.0f * pos.x) / float(width) - 1.0f;
  float y = 1.0f - (2.0f * pos.y) / float(height);

  glm::mat4 inverseViewProjection = inverseProjection * inverseView;

  glm::vec4 clipNear = glm::vec4(x, y, -1.0f, 1.0f);
  glm::vec4 clipFar = glm::vec4(x, y, 1.0f, 1.0f);
  glm::vec4 worldNear = inverseViewProjection * clipNear;
  glm::vec4 worldFar = inverseViewProjection * clipFar;

  worldNear /= worldNear.w;
  worldFar /= worldFar.w;

  nearPoint = glm::vec3(worldNear);
  farPoint = glm::vec3(worldFar);
}

std::array<glm::vec3, 8> createFrustumPoints(const glm::mat4& inverseView, const glm::mat4& inverseProjection, 
                                             const glm::vec2& startPos, const glm::vec2& endPos,
                                             int width, int height){
  std::array<glm::vec3, 8> points;

  glm::vec2 minPos = glm::vec2(glm::min(startPos, endPos));
  glm::vec2 maxPos = glm::vec2(glm::max(startPos, endPos));

  glm::vec2 BL = glm::vec2(minPos);
  glm::vec3 nearBL;
  glm::vec3 farBL;
  convertV2toFrustumPoints(BL, width, height, inverseView, inverseProjection, nearBL, farBL);
  points[NEAR_BL] = nearBL;
  points[FAR_BL] = farBL;

  glm::vec2 BR = glm::vec2(maxPos.x, minPos.y);
  glm::vec3 nearBR;
  glm::vec3 farBR;
  convertV2toFrustumPoints(BR, width, height, inverseView, inverseProjection, nearBR, farBR);
  points[NEAR_BR] = nearBR;
  points[FAR_BR] = farBR;

  glm::vec2 TL = glm::vec2(minPos.x, maxPos.y);
  glm::vec3 nearTL;
  glm::vec3 farTL;
  convertV2toFrustumPoints(TL, width, height, inverseView, inverseProjection, nearTL, farTL);
  points[NEAR_TL] = nearTL;
  points[FAR_TL] = farTL;

  glm::vec2 TR = glm::vec2(maxPos);
  glm::vec3 nearTR;
  glm::vec3 farTR;
  convertV2toFrustumPoints(TR, width, height, inverseView, inverseProjection, nearTR, farTR);
  points[NEAR_TR] = nearTR;
  points[FAR_TR] = farTR;

  return points;
}

Plane createPlane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c){
  Plane plane;
  glm::vec3 ab = b - a;
  glm::vec3 ac = c - a;

  plane.normal = glm::normalize(glm::cross(ab, ac));
  plane.d = -glm::dot(plane.normal, a);

  return plane;
}

Frustum createFrustumPlanes(const std::array<glm::vec3, 8>& points){
  Frustum f;
  
  f.planes[NEAR] = createPlane(points[NEAR_BL], points[NEAR_BR], points[NEAR_TR]);
  f.planes[FAR] = createPlane(points[FAR_BR], points[FAR_BL], points[FAR_TL]);
  f.planes[LEFT] = createPlane(points[FAR_BL], points[NEAR_BL], points[NEAR_TL]);
  f.planes[RIGHT] = createPlane(points[NEAR_BR], points[FAR_BR], points[FAR_TR]);
  f.planes[TOP] = createPlane(points[NEAR_TL], points[NEAR_TR], points[FAR_TR]);
  f.planes[BOTTOM] = createPlane(points[FAR_BL], points[FAR_BR], points[NEAR_BR]);

  return f;
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

bool outsidePlane(const Plane& plane, const AABB& hitbox){
  glm::vec3 positive;

  positive.x = hitbox.min.x;
  positive.y = hitbox.min.y;
  positive.z = hitbox.min.z;

  if (plane.normal.x >= 0)
    positive.x = hitbox.max.x;

  if (plane.normal.y >= 0)
    positive.y = hitbox.max.y;

  if (plane.normal.z >= 0)
    positive.z = hitbox.max.z;

  return glm::dot(plane.normal, positive) + plane.d < 0;
}

bool selectionBoxPickTile(const std::array<glm::vec3, 8>& points, const std::vector<std::unique_ptr<Tile>>& tiles, global::players player){
  bool selection = false; 

  Frustum frustum = createFrustumPlanes(points);

  for (const auto& tile : tiles){
    AABB hitbox = convertTileToAABB(tile);

    bool out = false;
    for (const auto& plane : frustum.planes){
      if (outsidePlane(plane, hitbox)){
        out = true;
        break;
      }
    }

    if (!out){
      std::cout << "in" << std::endl;
      tile->selected = player;
      selection = true;
    }

  }

  return selection;
}

}

