#include "../include/update.hpp"

#include <GLES3/gl3.h>
#include "../include/camera.hpp"

void update(){
  camera::cameras.normal->rotate();
  camera::cameras.normal->move();
}
