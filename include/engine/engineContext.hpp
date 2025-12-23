#ifndef ENGINE_CONTEXT_H
#define ENGINE_CONTEXT_H

#include <unordered_map>
#include <memory>

#include "geometry.hpp"
#include "framebuffer.hpp"
#include "texture.hpp"
#include "input.hpp"

class Shader;
class Model;

namespace engineContext {
  struct SetupConfig {
    int width;
    int height;
    int fps;
  };
};

class EngineContext {
public:
  EngineContext();
  ~EngineContext();

  int width;
  int height;
  unsigned int fps;
  float logicIntervalTime;

  long long lastUpdateTime = 0;
  double excessTime = 0.0;

  Input input;

  void addFrameBuffer(const std::string& name, Framebuffer framebuffer);
  void addGeometry(const std::string& name, Geometry geometry);
  void addTexture(const std::string& name, Texture texture);
  void addShader(const std::string& name, Shader shader);
  void addModel(const std::string& name, Model model);

  Framebuffer& getFramebuffer(const std::string& name);
  Geometry& getGeometry(const std::string& name);
  Texture& getTexture(const std::string& name);
  Shader& getShader(const std::string& name);
  Model* getModel(const std::string& name);

  void setupConfig(engineContext::SetupConfig config);
  void setupFramebuffers(engineContext::SetupConfig config);
  void setupGeometries();
  void setupTextures();
  void setupShaders();
  void setupModels();

  void setup(engineContext::SetupConfig config);

private:
  std::unordered_map<std::string, Shader> shaders;
  std::unordered_map<std::string, Framebuffer> framebuffers;
  std::unordered_map<std::string, Geometry> geometries;
  std::unordered_map<std::string, Texture> textures;
  std::unordered_map<std::string, Model> models;
};

#endif
