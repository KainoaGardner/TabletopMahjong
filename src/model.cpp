#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../include/model.hpp"
#include <iostream>
#include "../include/shader.hpp"


namespace model {
  Models model;

  void setup(){
    model.chun = std::make_unique<Model>("../assets/models/chun.glb");
  }
}

Model::Model(const std::string& path){
  loadModel(path);
}

Model::~Model(){
  for (auto& mesh : meshes){
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ebo);

    if (mesh.textureIndex) {
      glDeleteTextures(1, &mesh.textureIndex);
    }
  }
}

void Model::loadModel(const std::string& path){
  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err, warn;

  bool ok;
  if (path.size() > 4 && path.substr(path.size() - 4) == ".glb"){
    ok = loader.LoadBinaryFromFile(&model,&err,&warn,path);
  }else{
    ok = loader.LoadASCIIFromFile(&model,&err,&warn,path);
  }

  if (!warn.empty()) std::cout << "Warning: " << warn << std::endl;
  if (!err.empty()) std::cerr << "Error: " << err << std::endl;
  if (!ok) std::cerr << "Failed to load model: " << path << std::endl;

  for (const auto& mesh : model.meshes) {
    loadMesh(model,mesh);
  }
}


void Model::loadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh){
  for (const auto& primitive : mesh.primitives){
    model::Mesh mesh;

    //position
    const auto posIt = primitive.attributes.find("POSITION");
    if (posIt == primitive.attributes.end()) continue;
    const tinygltf::Accessor& posAccessor = model.accessors[posIt->second];
    const tinygltf::BufferView& posView = model.bufferViews[posAccessor.bufferView];
    const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
    const float* positions = reinterpret_cast<const float*>(&posBuffer.data[posView.byteOffset + posAccessor.byteOffset]);

    //uv
    const float* texCoords = nullptr;
    size_t uvCount = 0;
    auto uvIt = primitive.attributes.find("TEXCOORD_0");
    if (uvIt != primitive.attributes.end()) {
      const tinygltf::Accessor& uvAccessor = model.accessors[uvIt->second];
      const tinygltf::BufferView& uvView = model.bufferViews[uvAccessor.bufferView];
      const tinygltf::Buffer& uvBuffer = model.buffers[uvView.buffer];
      texCoords = reinterpret_cast<const float*>(&uvBuffer.data[uvView.byteOffset + uvAccessor.byteOffset]);
      uvCount = uvAccessor.count;
    }


    //create vertices
    std::vector<model::Vertex> vertices(posAccessor.count);
    for (size_t i = 0; i < posAccessor.count; ++i){
      vertices[i].position[0] = positions[i * 3 + 0];
      vertices[i].position[1] = positions[i * 3 + 1];
      vertices[i].position[2] = positions[i * 3 + 2];

      if (texCoords && i < uvCount){
        vertices[i].texCoord[0] = texCoords[i * 2 + 0];
        vertices[i].texCoord[1] = texCoords[i * 2 + 1];
      }else{
        vertices[i].texCoord[0] = 0.0f;
        vertices[i].texCoord[1] = 0.0f;
      }
    }

    //indices
    const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
    const tinygltf::BufferView& indexView = model.bufferViews[indexAccessor.bufferView];
    const tinygltf::Buffer& indexBuffer = model.buffers[indexView.buffer];

    std::vector<unsigned short> indices(indexAccessor.count);
    memcpy(indices.data(),
           &indexBuffer.data[indexView.byteOffset + indexAccessor.byteOffset],
           indexAccessor.count * sizeof(unsigned short));

    mesh.indexCount = indices.size();


    //textures
    if (!model.textures.empty() && primitive.material >= 0) {
      const auto& material = model.materials[primitive.material];
      if (material.pbrMetallicRoughness.baseColorTexture.index >= 0) {
        int texIndex = material.pbrMetallicRoughness.baseColorTexture.index;
        int imageIndex = model.textures[texIndex].source;
        mesh.textureIndex = loadTexture(model.images[imageIndex]);
      }
    }


    //upload to gpu
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER,mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(model::Vertex),vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned short),indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(model::Vertex), (void*)offsetof(model::Vertex,position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(model::Vertex), (void*)offsetof(model::Vertex,texCoord));

    glBindVertexArray(0);
    meshes.push_back(mesh);
  }
}

GLuint Model::loadTexture(const tinygltf::Image& image){
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  GLenum format = image.component == 3 ? GL_RED : GL_RGBA;

  glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.image.data());
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return tex;
}

void Model::draw() const {
  shader::shader.model->use();

  for (const auto& mesh : meshes) {
    glBindTexture(GL_TEXTURE_2D, mesh.textureIndex);
    glBindVertexArray(mesh.vao);
    shader::shader.screen->setInt("uDiffuse0",0);

    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_SHORT, 0);
  }

  glBindVertexArray(0);
}


