#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>
#include <vector>
#include <array>

#include "../include/engine/config.hpp"

class Tile;

namespace collision {

struct AABB {
  glm::vec3 min;
  glm::vec3 max;
};

struct Plane {
  glm::vec3 normal;
  float d;
};

struct Frustum {
  std::array<Plane, 6> planes;
};


enum FrustumPlane {
  LEFT = 0,
  RIGHT,
  TOP,
  BOTTOM,
  NEAR,
  FAR,
};


enum FrustumCorner {
  NEAR_BL = 0,
  NEAR_BR,
  NEAR_TL,
  NEAR_TR,
  FAR_BL,
  FAR_BR,
  FAR_TL,
  FAR_TR,
};

void computeMouseRay(float mouseX, float mouseY, int width, int height,
                     const glm::mat4& inverseView, const glm::mat4& inverseProjection, const glm::vec3& cameraPos,
                     glm::vec3& rayOrigin, glm::vec3& rayDir);

void convertV2toFrustumPoints(const glm::vec2& pos, int width, int height,
                            const glm::mat4& inverseView, const glm::mat4& inverseProjection,
                            glm::vec3& nearPoint, glm::vec3& farPoint);

std::array<glm::vec3, 8> createFrustumPoints(const glm::mat4& inverseView, const glm::mat4& inverseProjection, 
                                             const glm::vec2& startPos, const glm::vec2& endPos,
                                             int width, int height);

Plane createPlane(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& center);
Frustum createFrustumPlanes(const std::array<glm::vec3, 8>& points);

AABB convertTileToAABB(const std::unique_ptr<Tile>& tile);
AABB convertTileToWorldAABB(const std::unique_ptr<Tile>& tile);

bool rayIntersectAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const AABB& box, float& tHit);
bool outsidePlane(const Plane& plane, const AABB& hitbox);

Tile* pickTile(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const std::vector<std::unique_ptr<Tile>>& tiles);
bool selectionBoxPickTile(const std::array<glm::vec3, 8>& points, const std::vector<std::unique_ptr<Tile>>& tiles, global::players player);

}


#endif
