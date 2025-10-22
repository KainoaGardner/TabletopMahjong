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

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

void mainLoop();

float x = 0.0f;
float y = 0.0f;
float z = 0.0f;

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

  input::setup();
  shader::setup();
  geometry::setup();
  framebuffer::setup();
  camera::setup();
  model::setup();

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

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer::framebuffers.screen.fbo);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glm::mat4 view = camera::cameras.normal->getViewMatrix();
  glm::mat4 projection = camera::cameras.normal->getProjectionMatrix();


  shader::shader.model->use();
  glm::mat4 model = glm::mat4(1.0f);
  glm::vec3 posTest = glm::vec3(x,y,z);
  model = glm::translate(model, posTest);
  glm::vec3 size = glm::vec3(0.1f);
  model = glm::scale(model, size);
  shader::shader.model->setMatrix4fv("uModel", model);
  shader::shader.model->setMatrix4fv("uView", view);
  shader::shader.model->setMatrix4fv("uProjection", projection);
  model::model.chun->draw();

  // shader::shader.normal->use();
  // glBindVertexArray(geometry::geometry.cube.vao);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry::geometry.cube.ebo);
  // glm::mat4 model = glm::mat4(1.0f);
  //
  // glm::vec3 posTest = glm::vec3(x,y,z);
  // model = glm::translate(model, posTest);
  // glm::vec3 size = glm::vec3(1.0f);
  // model = glm::scale(model, size);
  //
  // shader::shader.normal->setMatrix4fv("uModel", model);
  // shader::shader.normal->setMatrix4fv("uView", view);
  // shader::shader.normal->setMatrix4fv("uProjection", projection);
  //
  // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader::shader.screen->use();
  glBindVertexArray(geometry::geometry.screen.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry::geometry.screen.ebo);
  glDisable(GL_DEPTH_TEST);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, framebuffer::framebuffers.screen.texture);
  shader::shader.screen->setInt("uTexture0",0);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  // render();
}


