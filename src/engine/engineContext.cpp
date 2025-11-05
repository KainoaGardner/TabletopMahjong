#include "engine/engineContext.hpp"

EngineContext::EngineContext(){
}

EngineContext::~EngineContext(){
}

//Framebuffers
void EngineContext::addFrameBuffer(const std::string& name, std::shared_ptr<Framebuffer> framebuffer){
  framebuffers[name] = std::move(framebuffer);
}

std::shared_ptr<Framebuffer> EngineContext::getFramebuffer(const std::string& name){
  return framebuffers.at(name);
}

//Geometries
void EngineContext::addGeometry(const std::string& name, std::shared_ptr<Geometry> geometry){
  geometries[name] = std::move(geometry);
}

std::shared_ptr<Geometry> EngineContext::getGeometry(const std::string& name){
  return geometries.at(name);
}


//Textures
void EngineContext::addTexture(const std::string& name, std::shared_ptr<Texture> texture){
  textures[name] = std::move(texture);
}

std::shared_ptr<Texture> EngineContext::getTexture(const std::string& name){
  return textures.at(name);
}

//Shaders
void EngineContext::addShader(const std::string& name, std::shared_ptr<Shader> shader){
  shaders[name] = std::move(shader);
}

std::shared_ptr<Shader> EngineContext::getShader(const std::string& name){
  return shaders.at(name);
}


void EngineContext::setup(engineContext::SetupConfig config){
  //Framebuffers
  auto screenFramebuffer = std::make_shared<Framebuffer>(framebuffer::create(config.width, config.height));
  addFrameBuffer("screen", screenFramebuffer);

  //Geometries
  auto planeGeometry = std::make_shared<Geometry>(geometry::createPlane());
  auto cubeGeometry = std::make_shared<Geometry>(geometry::createCube());
  auto cubemapGeometry = std::make_shared<Geometry>(geometry::createCubemap());
  auto screenGeometry = std::make_shared<Geometry>(geometry::createScreen());

  addGeometry("plane", planeGeometry);
  addGeometry("cube", cubeGeometry);
  addGeometry("cubemap", cubemapGeometry);
  addGeometry("screen", screenGeometry);

  //textures
  auto matTexture = std::make_shared<Texture>(texture::createTexture("../assets/textures/table/mat1.jpg"));
  auto woodTexture = std::make_shared<Texture>(texture::createTexture("../assets/textures/table/woodDiff.jpg"));

  std::vector<std::string> sky4Textures = {
    "../assets/textures/cubemaps/sky4/posx.png",
    "../assets/textures/cubemaps/sky4/negx.png",
    "../assets/textures/cubemaps/sky4/posy.png",
    "../assets/textures/cubemaps/sky4/negy.png",
    "../assets/textures/cubemaps/sky4/negz.png",
    "../assets/textures/cubemaps/sky4/posz.png",
  };
  auto sky4Texture = std::make_shared<Texture>(texture::createCubemap(sky4Textures));

  addTexture("mat", matTexture);
  addTexture("tableSide", woodTexture);
  addTexture("sky4", sky4Texture);

  //shaders
  auto normalShader = std::make_shared<Shader>("./assets/shaders/vertex.vert", "./assets/shaders/fragment.frag");
  auto screenShader = std::make_shared<Shader>("./assets/shaders/screen.vert", "./assets/shaders/screen.frag");
  auto modelShader = std::make_shared<Shader>("./assets/shaders/model.vert", "./assets/shaders/model.frag");
  auto cubemapShader = std::make_shared<Shader>("./assets/shaders/cubemap.vert", "./assets/shaders/cubemap.frag");
}

std::shared_ptr<EngineContext> engineCTX = std::make_shared<EngineContext>();
