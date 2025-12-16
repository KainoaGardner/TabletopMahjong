#include <GLES3/gl3.h>

#include <emscripten/html5_webgl.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include "../include/engine/engineContext.hpp"

#include "../include/engine/config.hpp"
#include "../include/util/glExtension.hpp"

#include "../include/engine/render.hpp"
#include "../include/engine/update.hpp"
// #include "../include/camera.hpp"
// #include "../include/tile.hpp"
// #include "../include/gameState.hpp"
// #include "../include/dice.hpp"

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

  engineContext::SetupConfig setupConfig = {
    // .width = 1920,
    // .height = 1080,
    // .width = 1600,
    // .height = 900,
    // .width = 1280,
    // .height = 720,
    // .width = 960,
    // .height = 540,
    .width = 1366,
    .height = 768,
    .fps = 60,
  };

  emscripten_set_canvas_element_size("#canvas", setupConfig.width, setupConfig.height);

  glExtensions::setup();
  engineCTX->setup(setupConfig);
  // gameState::setup();
  // camera::setup();
  // dice::setup();

  // tile::setup(tile::FourP);
  // tile::setup(tile::ThreeP);
  // int roll = dice::getDiceRoll() + dice::getDiceRoll();
  // tile::makeWalls();
  // tile::dealHands(roll);
  // tile::makeDeadWall(roll);

  glViewport(0, 0, engineCTX->width, engineCTX->height);
  emscripten_set_main_loop(mainLoop, 0, true);

  return 0;
}

void mainLoop(){
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  if (engineCTX->lastUpdateTime == 0){
    engineCTX->lastUpdateTime = currentTime;
  }

  long long timeDif = currentTime - engineCTX->lastUpdateTime;
  if (timeDif > global::maxTimeGap){
    timeDif = global::maxTimeGap;
  }

  engineCTX->lastUpdateTime = currentTime;
  engineCTX->excessTime += timeDif;

  while (engineCTX->excessTime >= engineCTX->logicIntervalTime) {
    update();
    engineCTX->excessTime -= engineCTX->logicIntervalTime;
  }
  
  render::main();
}
