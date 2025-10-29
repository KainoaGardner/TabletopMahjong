#include <GLES3/gl3.h>

#include <emscripten/html5_webgl.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include "../include/config.hpp"
#include "../include/render.hpp"
#include "../include/update.hpp"
#include "../include/shader.hpp"
#include "../include/geometry.hpp"
#include "../include/framebuffer.hpp"
#include "../include/camera.hpp"
#include "../include/input.hpp"
#include "../include/model.hpp"
#include "../include/texture.hpp"
#include "../include/glExtension.hpp"
#include "../include/tile.hpp"

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

void mainLoop();


int main(){
  EmscriptenWebGLContextAttributes attr;
  emscripten_webgl_init_context_attributes(&attr);
  attr.alpha = EM_TRUE;
  attr.depth = EM_TRUE;
  attr.stencil = EM_FALSE;
  attr.antialias = EM_TRUE;
  attr.majorVersion = 2;


  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx;
  ctx = emscripten_webgl_create_context("#canvas", &attr);
  if (ctx <= 0){
    emscripten_log(EM_LOG_ERROR, "Failed to create WebGL2 contex");
    return 1;
  }
  emscripten_webgl_make_context_current(ctx);

  int canvasWidth = config::gameConfig.width;
  int canvasHeight = config::gameConfig.height;

  emscripten_set_canvas_element_size("#canvas", canvasWidth,canvasHeight);

  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  config::gameConfig.lastUpdateTime = currentTime;

  glExtensions::setup();
  texture::setup();
  input::setup();
  shader::setup();
  geometry::setup();
  framebuffer::setup();
  camera::setup();
  model::setup();
  tile::setup(0);

  glViewport(0, 0, canvasWidth,canvasHeight);
  emscripten_set_main_loop(mainLoop, 0, true);

  return 0;
}

void mainLoop(){
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  // std::cout << currentTime - config::gameConfig.lastUpdateTime << std::endl;
  // if (currentTime - config::gameConfig.lastUpdateTime >= config::gameConfig.logicIntervalTime) {
  //   update();
  //   config::gameConfig.lastUpdateTime += config::gameConfig.logicIntervalTime;
  // }

  update();
  render::main();
}
