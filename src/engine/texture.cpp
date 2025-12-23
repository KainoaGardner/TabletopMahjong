#include "engine/texture.hpp"
#include "../include/stb_image.h"

#include <GLES3/gl3.h>
#include <iostream>

namespace texture {
GLuint createTexture(const char *path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponets;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponets, 0);

  if (data) {
    GLenum format = GL_RED;
    if (nrComponets == 3) {
      format = GL_RGB;
    } else if (nrComponets == 4) {
      format = GL_RGBA;
    } 

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cout << "Texture not loaded path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}


GLuint createCubemap(const std::vector<std::string>& textures) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrComponets;
  for (unsigned int i = 0; i < textures.size(); i++) {
    unsigned char *data =
        stbi_load(textures[i].c_str(), &width, &height, &nrComponets, 0);
    if (data) {
      GLenum format = GL_RED;
      if (nrComponets == 3) {
        format = GL_RGB;
      } else if (nrComponets == 4) {
        format = GL_RGBA;
      } 

      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height,
                   0, format, GL_UNSIGNED_BYTE, data);

      stbi_image_free(data);
    } else {
      std::cout << "Cubemap texture failed" << textures[i] << std::endl;
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}
}
