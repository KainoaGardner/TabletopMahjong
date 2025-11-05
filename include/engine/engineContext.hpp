#ifndef ENGINE_CONTEXT_H
#define ENGINE_CONTEXT_H

#include <unordered_map>
#include <string>
#include <memory>

class Shader;
class Model;
#include "geometry.hpp"
#include "framebuffer.hpp"
#include "texture.hpp"

namespace engineContext {
  struct SetupConfig {
    int width;
    int height;
  };
};

class EngineContext {
public:
  EngineContext();
  ~EngineContext();


  void addFrameBuffer(const std::string& name, std::shared_ptr<Framebuffer> framebuffer);
  void addGeometry(const std::string& name, std::shared_ptr<Geometry> geometry);
  void addTexture(const std::string& name, std::shared_ptr<Texture> texture);
  void addShader(const std::string& name, std::shared_ptr<Shader> shader);

  std::shared_ptr<Framebuffer> getFramebuffer(const std::string& name);
  std::shared_ptr<Geometry> getGeometry(const std::string& name);
  std::shared_ptr<Texture> getTexture(const std::string& name);
  std::shared_ptr<Shader> getShader(const std::string& name);

  void setup(engineContext::SetupConfig config);

private:
  std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
  std::unordered_map<std::string, std::shared_ptr<Framebuffer>> framebuffers;
  std::unordered_map<std::string, std::shared_ptr<Geometry>> geometries;
  std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};



extern std::shared_ptr<EngineContext> engineCTX;

#endif
