#include "../include/glExtension.hpp"

#include <GLES3/gl3.h>
#include <cstring>

namespace glExtensions {
void setup(){
  const char* ext = (const char*)glGetString(GL_EXTENSIONS);
  bool aniso = ext && strstr(ext, "GL_EXT_texture_filter_anisotropic");
  
  if (aniso){
    #ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
    #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
    #define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
    #endif
  }
}
}
