#include <GLES3/gl3.h>

#include <emscripten/html5_webgl.h>
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include "../include/engine/engineContext.hpp"

#include "../include/app.hpp"
#include "../include/engine/config.hpp"
#include "../include/util/glExtension.hpp"

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

void mainLoop(void* arg);

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



  glExtensions::setup();

  App* app = new App();

  emscripten_set_canvas_element_size("#canvas", app->engineCTX.width, app->engineCTX.height);
  // gameState::setup();

  glViewport(0, 0, app->engineCTX.width, app->engineCTX.height);
  emscripten_set_main_loop_arg(mainLoop, app, 0, true);

  return 0;
}

void mainLoop(void* arg){
  App* app = static_cast<App*>(arg);

  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  if (app->engineCTX.lastUpdateTime == 0){
    app->engineCTX.lastUpdateTime = currentTime;
  }

  long long timeDif = currentTime - app->engineCTX.lastUpdateTime;
  if (timeDif > global::maxTimeGap){
    timeDif = global::maxTimeGap;
  }

  app->engineCTX.lastUpdateTime = currentTime;
  app->engineCTX.excessTime += timeDif;

  while (app->engineCTX.excessTime >= app->engineCTX.logicIntervalTime) {
    app->update();
    app->engineCTX.excessTime -= app->engineCTX.logicIntervalTime;
  }
  
  app->render();
}
