#ifndef RENDER_H
#define RENDER_H


#include <glm/glm.hpp>
#include <vector>

class EngineContext;
class Game;
class Shader;
class Tile;

namespace render {
  void main(float a, EngineContext& engineCTX, Game& gameCTX);

  glm::mat4 getInterpCameraView(float a, Game& gameCTX);
  glm::mat4 getInterpCameraProj(float a, Game& gameCTX, EngineContext& engineCTX);

  glm::vec3 linearInterp(const glm::vec3& a, const glm::vec3& b, float alpha);
  float linearInterp(float a, float b, float alpha);

  void tiles(EngineContext& engineCTX, const std::vector<std::unique_ptr<Tile>>& tiles, const glm::mat4& view, const glm::mat4& projection);
  void selectHighlight(EngineContext& engineCTX, const std::vector<std::unique_ptr<Tile>>& tiles, const glm::mat4& view, const glm::mat4& projection);
  void dice(EngineContext& engineCTX, const glm::mat4& view, const glm::mat4& projection);
  void table(EngineContext& engineCTX, const glm::mat4& view, const glm::mat4& projection);
  void mat(EngineContext& engineCTX, const glm::mat4& view, const glm::mat4& projection);
  void cubemap(EngineContext& engineCTX, const glm::mat4& view, const glm::mat4& projection);


  void click(EngineContext& engineCTX, glm::vec3 clickPos, const glm::mat4& view, const glm::mat4& projection);
  void screen(EngineContext& engineCTX);
}

#endif
