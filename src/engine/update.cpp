#include "../include/update.hpp"

#include <GLES3/gl3.h>
#include "../include/camera.hpp"
#include "../include/input.hpp"

void update(){

  camera::switchCamera();
  camera::cameras[camera::curr]->update();

  input::updateMouse();
}
