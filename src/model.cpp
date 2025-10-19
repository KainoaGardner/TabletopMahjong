#include "../include/model.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/tiny_gltf.h"

#include <iostream>

namespace model {

void loadGLB(const std::string& path){
  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err, warn;
  
  bool ret = loader.LoadBinaryFromFile(&model,&err,&warn,path);
  if (!warn.empty()) std::cout << "Warning: " << warn << std::endl;
  if (!err.empty()) std::cout << "Error: " << err << std::endl;
  if (!ret) {
    std::cout << "Failed to load: " << path << std::endl;
    return;
  }

  std::cout << "Meshes:" << model.meshes.size() << std::endl;
  std::cout << "Nodes:" << model.nodes.size() << std::endl;
}

} // namespace model
