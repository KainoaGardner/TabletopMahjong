#ifndef RENDER_H
#define RENDER_H


#include <glm/glm.hpp>

namespace render {
  void main();

  void tiles(const glm::mat4& view, const glm::mat4& projection);
  void dice(const glm::mat4& view, const glm::mat4& projection);
  void table(const glm::mat4& view, const glm::mat4& projection);
  void mat(const glm::mat4& view, const glm::mat4& projection);

  void screen();
}

#endif
