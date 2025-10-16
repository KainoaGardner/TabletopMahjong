#include "../include/render.hpp"

#include <GLES3/gl3.h>

void render(){
  glClearColor(1.0f,0.1f,0.1f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
