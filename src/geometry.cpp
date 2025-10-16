#include "../include/geometry.hpp"

float CUBE_VERTICES[120] = {
    // front
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  

    //back
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   

    // up
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,  
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f,  
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   

    //down
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f,   
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,    
    0.5f, -0.5f, 0.5f, 0.0f, 1.0f,   

    // left
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f,   
    -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,  

    //right
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f,   
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f,  
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f,    
};

unsigned int CUBE_INDICES[36] = {
    // front
    0, 3, 2, 2, 1, 0,
    //back
    4, 5, 6, 6, 7, 4,
    // left
    11, 8, 9, 9, 10, 11,
    //right
    12, 13, 14, 14, 15, 12,
    // bottom
    16, 17, 18, 18, 19, 16,
    //top
    20, 21, 22, 22, 23, 20};


float SCREEN_VERTICES[20] = {
    // positions       // texture coords
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top right
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f  // top left
};

unsigned int SCREEN_INDICES[6] = {
    0, 1, 3, // first tri
    1, 2, 3, // second tri
};


Geometry createCube() {
  Geometry geometry;
  glGenVertexArrays(1, &geometry.vao);
  glBindVertexArray(geometry.vao);

  glGenBuffers(1, &geometry.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICES), CUBE_VERTICES, GL_STATIC_DRAW);

  glGenBuffers(1, &geometry.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CUBE_INDICES), CUBE_INDICES, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return geometry;
}

Geometry createScreen() {
  Geometry geometry;
  glGenVertexArrays(1, &geometry.vao);
  glBindVertexArray(geometry.vao);

  glGenBuffers(1, &geometry.vbo);

  glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(SCREEN_VERTICES), SCREEN_VERTICES, GL_STATIC_DRAW);

  glGenBuffers(1, &geometry.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SCREEN_INDICES), SCREEN_INDICES, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0); 
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return geometry;
}

namespace geometry {
Geometries geometry;
}
