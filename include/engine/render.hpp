#ifndef RENDER_H
#define RENDER_H


#include <glm/glm.hpp>
#include <vector>

class EngineContext;
class Game;
class Shader;
class Tile;

namespace render {
  void main(EngineContext& engineCTX, Game& gameCTX);

  void tiles(Shader& shader, std::vector<Tile>& tiles, const glm::mat4& view, const glm::mat4& projection);
  void dice(EngineContext& engineCTX, const glm::mat4& view, const glm::mat4& projection);
  void table(EngineContext& engineCTX, const glm::mat4& view, const glm::mat4& projection);
  void mat(EngineContext& engineCTX, const glm::mat4& view, const glm::mat4& projection);
  void cubemap(EngineContext& engineCTX, const glm::mat4& view, const glm::mat4& projection);

  void screen(EngineContext& engineCTX);
}

#endif
