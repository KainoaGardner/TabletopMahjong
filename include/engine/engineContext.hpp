#ifndef ENGINE_CONTEXT_H
#define ENGINE_CONTEXT_H

#include <unordered_map>
#include <memory>

#include "geometry.hpp"
#include "framebuffer.hpp"
#include "texture.hpp"
#include "settings.hpp"
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
  settings::Settings settings;

  Framebuffer* addFrameBuffer(const std::string& name, std::unique_ptr<Framebuffer> framebuffer);
  Geometry* addGeometry(const std::string& name, std::unique_ptr<Geometry> geometry);
  Texture* addTexture(const std::string& name, std::unique_ptr<Texture> texture);
  Shader* addShader(const std::string& name, std::unique_ptr<Shader> shader);
  Model* addModel(const std::string& name, std::unique_ptr<Model> model);

  Framebuffer* getFramebuffer(const std::string& name);
  Geometry* getGeometry(const std::string& name);
  Texture* getTexture(const std::string& name);
  Shader* getShader(const std::string& name);
  Model* getModel(const std::string& name);

  void setupConfig(engineContext::SetupConfig config);
  void setupSettings(settings::Settings settings);
  void setupFramebuffers(engineContext::SetupConfig config);
  void setupGeometries();
  void setupTextures();
  void setupShaders();
  void setupModels();
  void setupFonts();

  void setup(engineContext::SetupConfig config, settings::Settings settings);

private:
  std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
  std::unordered_map<std::string, std::unique_ptr<Framebuffer>> framebuffers;
  std::unordered_map<std::string, std::unique_ptr<Geometry>> geometries;
  std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
  std::unordered_map<std::string, std::unique_ptr<Model>> models;
};

#endif
