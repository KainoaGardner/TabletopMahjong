#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GLES3/gl3.h>

struct Framebuffer {
    GLuint fbo;
    GLuint texture;
};

namespace framebuffer {
  Framebuffer create(int width, int height);
  void resize(Framebuffer& framebuffer, int width, int height);
  Framebuffer createColorBuffer(int width, int height, bool withAlpha);
};

#endif
