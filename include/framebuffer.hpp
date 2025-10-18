#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GLES3/gl3.h>


namespace framebuffer {
  struct Framebuffer {
    GLuint fbo;
    GLuint texture;
  };

  struct Framebuffers {
    Framebuffer screen;
  };


  Framebuffer createScreenFrameBuffer();

  void setup();

extern Framebuffers framebuffers;
};

#endif
