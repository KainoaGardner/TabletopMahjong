#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GLES3/gl3.h>



namespace geometry {
  extern float CUBE_VERTICES[120];
  extern float SCREEN_VERTICES[20];
  
  extern unsigned int CUBE_INDICES[36];
  extern unsigned int SCREEN_INDICES[6];


  struct Geometry {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
  };

  struct Geometries {
    Geometry cube;
    Geometry screen;
  };

  Geometry createCube();
  Geometry createScreen();

  void setup();


extern Geometries geometry;
} // namespace geometry


#endif
