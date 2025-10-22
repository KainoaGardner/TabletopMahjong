#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <memory>

#include <GLES3/gl3.h>

#include "../include/tiny_gltf.h"


namespace model {
struct Vertex {
  float position[3];
  float texCoord[2];
};

struct Mesh {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;

  int indexCount;
  GLuint textureIndex;
};

} // namespace model


class Model {
public:
  Model(const std::string& path);
  ~Model();

  void draw() const;

private:
  std::vector<model::Mesh> meshes;

  void loadModel(const std::string& path);
  void loadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh);
  GLuint loadTexture(const tinygltf::Image& image);
};


namespace model {
struct Models {
  std::unique_ptr<Model> chun;
};

void setup();
  
extern Models model;
}


#endif
