#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GLES3/gl3.h>

struct Geometry {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
};

namespace geometry {
  extern float PLANE_VERTICES[20];

  extern float CUBE_VERTICES[120];
  extern float CUBE_MAP_VERTICES[72];

  extern float SCREEN_VERTICES[20];

  extern float QUAD_VERTICES[8];
  
  extern unsigned int CUBE_INDICES[36];
  extern unsigned int SCREEN_INDICES[6];

  Geometry createPlane();
  Geometry createCube();
  Geometry createCubemap();
  Geometry createScreen();
  Geometry createQuad();
  Geometry createLine();
} 

#endif
