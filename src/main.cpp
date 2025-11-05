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
#include "../include/gameState.hpp"
#include "../include/dice.hpp"

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

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

  glExtensions::setup();
  texture::setup();
  input::setup();
  shader::setup();
  geometry::setup();
  framebuffer::setup();
  gameState::setup();
  camera::setup();
  dice::setup();
  model::setup();
  tile::setup(tile::FourP);
  // tile::setup(tile::ThreeP);

  int roll = dice::getDiceRoll() + dice::getDiceRoll();
  tile::makeWalls();
  tile::dealHands(roll);
  tile::makeDeadWall(roll);

  glViewport(0, 0, canvasWidth,canvasHeight);
  emscripten_set_main_loop(mainLoop, 0, true);

  return 0;
}

void mainLoop(){
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  if (config::gameConfig.lastUpdateTime == 0){
    config::gameConfig.lastUpdateTime = currentTime;
  }

  long long timeDif = currentTime - config::gameConfig.lastUpdateTime;
  if (timeDif > 250) timeDif = 250;

  config::gameConfig.lastUpdateTime = currentTime;
  config::gameConfig.excessTime += timeDif;

  while (config::gameConfig.excessTime >= config::gameConfig.logicIntervalTime) {
    update();
    config::gameConfig.excessTime -= config::gameConfig.logicIntervalTime;
  }
  

  render::main();
}
