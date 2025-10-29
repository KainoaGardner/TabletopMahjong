#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <memory>

#include <GLES3/gl3.h>
#include <glm/glm.hpp>

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


const glm::vec3 tableSideShortScale = glm::vec3(0.69f, 0.03f, 0.03f);
const glm::vec3 tableSideLongScale = glm::vec3(0.75f, 0.03f, 0.03f);
const glm::vec3 matScale = glm::vec3(0.69f, 0.005f, 0.69f);
const glm::vec3 diceScale = glm::vec3(0.016f);

const float tileScaleFactor = 1.14;
const glm::vec3 tileScale = glm::vec3(0.0245f, 0.0152f, 0.033f) * tileScaleFactor;
const glm::vec2 tileMapRatio = glm::vec2(model::tileScale.z / model::tileScale.x * 0.1 ,0.1f);
} // namespace model



class Model {
public:
  Model(const std::string& path);
  ~Model();

  void draw() const;
  std::vector<model::Mesh> meshes;
private:

  void loadModel(const std::string& path);
  void loadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh);
  GLuint loadTexture(const tinygltf::Image& image);
};


namespace model {
struct Models {
  std::unique_ptr<Model> tile;

  std::unique_ptr<Model> dice;
};

void setup();
  
extern Models model;
}


#endif



