#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>
#include <vector>

class Tile;

namespace collision {

struct AABB {
  glm::vec3 min;
  glm::vec3 max;
};

void computeMouseRay(float mouseX, float mouseY, int width, int height,
                     const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos,
                     glm::vec3& rayOrigin, glm::vec3& rayDir);

AABB convertTileToAABB(const std::unique_ptr<Tile>& tile);

bool rayIntersectAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const AABB& box, float& tHit);

Tile* pickTile(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const std::vector<std::unique_ptr<Tile>>& tiles);

}


#endif
