#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <GLES3/gl3.h>

namespace texture {
struct Texture {
  GLuint texture;
};

struct Textures {
  Texture mat;
  Texture test;
  Texture tableSide;
};

GLuint loadTexture(const char *path);
GLuint loadCubemap(const std::vector<std::string>& textures);

void setup();

extern Textures texture;
}



#endif
