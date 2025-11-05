#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <GLES3/gl3.h>

struct Texture {
  GLuint texture;
};

namespace texture {
  GLuint createTexture(const char *path);
  GLuint createCubemap(const std::vector<std::string>& textures);
}

#endif
