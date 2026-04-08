#include "../include/engine/config.hpp"
#include "../include/engine/collision.hpp"

#include "../include/game/game.hpp"
#include "../include/game/tile.hpp"
#include "../include/game/lockon.hpp"

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

  glm::mat4 inverseViewProjection = inverseView * inverseProjection ;

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

Plane createPlane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& center){
  Plane plane;
  glm::vec3 ab = b - a;
  glm::vec3 ac = c - a;

  plane.normal = glm::normalize(glm::cross(ab, ac));
  plane.d = -glm::dot(plane.normal, a);

  float side = glm::dot(plane.normal, center) + plane.d;
  if (side > 0.0f) {
      plane.normal = -plane.normal;
      plane.d = -plane.d;
  }

  plane.normal = -plane.normal;
  plane.d = -plane.d;

  return plane;
}

Frustum createFrustumPlanes(const std::array<glm::vec3, 8>& points){
  Frustum f;

  glm::vec3 center = (points[NEAR_BL] + points[FAR_TR]) * 0.5f;
  
  f.planes[NEAR] = createPlane(points[NEAR_BL], points[NEAR_BR], points[NEAR_TR], center); 
  f.planes[FAR] = createPlane(points[FAR_BR], points[FAR_BL], points[FAR_TL], center); 
  f.planes[LEFT] = createPlane(points[FAR_BL], points[NEAR_BL], points[NEAR_TL], center);
  f.planes[RIGHT] = createPlane(points[NEAR_BR], points[FAR_BR], points[FAR_TR], center);
  f.planes[TOP] = createPlane(points[NEAR_TL], points[NEAR_TR], points[FAR_TR], center);
  f.planes[BOTTOM] = createPlane(points[FAR_BL], points[FAR_BR], points[NEAR_BR], center);

  return f;
}

AABB createAABB(const glm::vec3& scale){
  AABB result;

  glm::vec3 halfSize = scale * 0.5f;

  result.min = -halfSize;
  result.max = halfSize;
  return result;
}

AABB createWorldAABB(const glm::vec3& scale, const glm::mat4& model){
  AABB result;

  glm::vec3 halfSize = scale;
  glm::vec3 localMin = -halfSize;
  glm::vec3 localMax =  halfSize;

  result.min = -halfSize;
  result.max = halfSize;

  glm::vec3 corners[8] = {
      {localMin.x, localMin.y, localMin.z},
      {localMax.x, localMin.y, localMin.z},
      {localMin.x, localMax.y, localMin.z},
      {localMax.x, localMax.y, localMin.z},
      {localMin.x, localMin.y, localMax.z},
      {localMax.x, localMin.y, localMax.z},
      {localMin.x, localMax.y, localMax.z},
      {localMax.x, localMax.y, localMax.z},
  };

  glm::vec3 worldMin(FLT_MAX);
  glm::vec3 worldMax(-FLT_MAX);

  for (int i = 0; i < 8; i++) {
    glm::vec4 worldCorner = model * glm::vec4(corners[i], 1.0f);
    glm::vec3 wc = glm::vec3(worldCorner);

    worldMin = glm::min(worldMin, wc);
    worldMax = glm::max(worldMax, wc);
  }

  result.min = worldMin;
  result.max = worldMax;

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
    AABB hitbox = createAABB(tile->scale);

    glm::mat4 model = tile->getModelMatrix();
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

  glm::vec3 corners[8] = {
    {hitbox.min.x, hitbox.min.y, hitbox.min.z},
    {hitbox.max.x, hitbox.min.y, hitbox.min.z},
    {hitbox.min.x, hitbox.max.y, hitbox.min.z},
    {hitbox.max.x, hitbox.max.y, hitbox.min.z},
    {hitbox.min.x, hitbox.min.y, hitbox.max.z},
    {hitbox.max.x, hitbox.min.y, hitbox.max.z},
    {hitbox.min.x, hitbox.max.y, hitbox.max.z},
    {hitbox.max.x, hitbox.max.y, hitbox.max.z},
  };

  int outsideCount = 0;
  for (int i = 0; i < 8; i++){
    if (glm::dot(plane.normal, corners[i]) + plane.d < 0){
      outsideCount++;
    }
  }

  return outsideCount == 8;

  // glm::vec3 positive = hitbox.min;
  //
  // if (plane.normal.x >= 0)
  //   positive.x = hitbox.max.x;
  //
  // if (plane.normal.y >= 0)
  //   positive.y = hitbox.max.y;
  //
  // if (plane.normal.z >= 0)
  //   positive.z = hitbox.max.z;
  //
  // return glm::dot(plane.normal, positive) + plane.d < 0;
}

bool selectionBoxPickTile(const std::array<glm::vec3, 8>& points, const std::vector<std::unique_ptr<Tile>>& tiles, global::players player){
  bool selection = false; 
  Frustum frustum = createFrustumPlanes(points);


  for (const auto& tile : tiles){
    AABB hitbox = createWorldAABB(tile->scale, tile->getModelMatrix());

    bool out = false;
    for (const auto& plane : frustum.planes){
      if (outsidePlane(plane, hitbox)){
        out = true;
        break;
      }
    }

    if (!out){
      tile->selected = player;
      selection = true;
    }

  }

  return selection;
}

LockSpace* checkLockSpaceCollision(const Tile* tile, std::unique_ptr<LockSpace>* lockSpaces, size_t size){
  AABB tileHitbox = createWorldAABB(tile->scale, tile->getModelMatrix());

  LockSpace* result = nullptr;
  float closestDist = FLT_MAX;
  
  for (size_t i = 0; i < size; ++i){
    auto& lockSpace  = lockSpaces[i];
    AABB lockSpaceHitbox = createWorldAABB(lockSpace->scale, lockSpace->getModelMatrix());

    if (check2dAABBCollision(tileHitbox, lockSpaceHitbox)){
      float dist2 = glm::distance(glm::vec2(tile->position.x, tile->position.z), glm::vec2(lockSpace->position.x, lockSpace->position.z));
      if (dist2 < closestDist){
        closestDist = dist2;
        result = lockSpace.get();
      }
    }
  }

  return result;
}

LockSpace* checkAllLockSpaceCollisions(const Tile* tile,
  std::unique_ptr<LockSpace> handLockSpaces[global::handLockSpaceAmount],
  std::unique_ptr<LockSpace> discardLockSpaces[global::discardLockSpaceAmount],
  std::unique_ptr<LockSpace> callLockSpaces[global::callLockSpaceAmount],
  std::unique_ptr<LockSpace> yamaLockSpaces[global::yamaLockSpaceAmount]){

  LockSpace* result = nullptr;
  float closestDist = FLT_MAX;

  auto checkArray = [&](auto* arr, size_t size) {
    if (auto lockSpace = checkLockSpaceCollision(tile, arr, size)) {
      float dist2 = glm::distance(glm::vec2(tile->position.x, tile->position.z), glm::vec2(lockSpace->position.x, lockSpace->position.z));
      if (dist2 < closestDist){
        closestDist = dist2;
        result = lockSpace;
      }
    }
  };

  checkArray(handLockSpaces, global::handLockSpaceAmount);
  checkArray(discardLockSpaces, global::discardLockSpaceAmount);
  checkArray(callLockSpaces, global::callLockSpaceAmount);
  checkArray(yamaLockSpaces, global::yamaLockSpaceAmount);
  return result;
}

bool check2dAABBCollision(const AABB& a, const AABB& b){
  if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
  if (a.max.z < b.min.z || a.min.z > b.max.z) return false;

  return true;
}

}
