#include "engine/engineContext.hpp"
#include "engine/model.hpp"
#include "engine/shader.hpp"

EngineContext::EngineContext(){
}

EngineContext::~EngineContext(){
}

//Framebuffers
void EngineContext::addFrameBuffer(const std::string& name, Framebuffer framebuffer){
  framebuffers.emplace(name, std::move(framebuffer));
}

Framebuffer& EngineContext::getFramebuffer(const std::string& name){
  return framebuffers.at(name);
}

//Geometries
void EngineContext::addGeometry(const std::string& name, Geometry geometry){
  geometries.emplace(name, std::move(geometry));
}

Geometry& EngineContext::getGeometry(const std::string& name){
  return geometries.at(name);
}

//Textures
void EngineContext::addTexture(const std::string& name, Texture texture){
  textures.emplace(name, std::move(texture));
}

Texture& EngineContext::getTexture(const std::string& name){
  return textures.at(name);
}

//Shaders
void EngineContext::addShader(const std::string& name, Shader shader){
  shaders.emplace(name, std::move(shader));
}

Shader& EngineContext::getShader(const std::string& name){
  return shaders.at(name);
}


//Models
void EngineContext::addModel(const std::string& name, Model model){
  models.emplace(name, std::move(model));
}

Model& EngineContext::getModel(const std::string& name){
  return models.at(name);
}



void EngineContext::setupConfig(engineContext::SetupConfig config){
  width = config.width;
  height = config.height;
  fps = config.fps;
  logicIntervalTime = 1000.0f / fps;
}

void EngineContext::setupTextures(){
  Texture matTexture = Texture{.texture = texture::createTexture("../assets/textures/table/mat1.jpg")};
  Texture woodTexture = Texture{.texture = texture::createTexture("../assets/textures/table/woodDiff.jpg")};

  std::vector<std::string> sky4Textures = {
    "../assets/textures/cubemaps/sky4/posx.png",
    "../assets/textures/cubemaps/sky4/negx.png",
    "../assets/textures/cubemaps/sky4/posy.png",
    "../assets/textures/cubemaps/sky4/negy.png",
    "../assets/textures/cubemaps/sky4/negz.png",
    "../assets/textures/cubemaps/sky4/posz.png",
  };
  Texture sky4Texture = Texture{.texture = texture::createCubemap(sky4Textures)};

  addTexture("mat", matTexture);
  addTexture("tableSide", woodTexture);
  addTexture("sky4", sky4Texture);
}

void EngineContext::setupFramebuffers(engineContext::SetupConfig config){
  auto screenFramebuffer = Framebuffer(framebuffer::create(config.width, config.height));

  addFrameBuffer("screen", screenFramebuffer);
}

void EngineContext::setupGeometries(){
  auto planeGeometry =  Geometry(geometry::createPlane());
  auto cubeGeometry = Geometry(geometry::createCube());
  auto cubemapGeometry = Geometry(geometry::createCubemap());
  auto screenGeometry = Geometry(geometry::createScreen());

  addGeometry("plane", planeGeometry);
  addGeometry("cube", cubeGeometry);
  addGeometry("cubemap", cubemapGeometry);
  addGeometry("screen", screenGeometry);
}

void EngineContext::setupShaders(){
  auto normalShader = Shader("./assets/shaders/vertex.vert", "./assets/shaders/fragment.frag");
  auto screenShader = Shader("./assets/shaders/screen.vert", "./assets/shaders/screen.frag");
  auto modelShader = Shader("./assets/shaders/model.vert", "./assets/shaders/model.frag");
  auto cubemapShader = Shader("./assets/shaders/cubemap.vert", "./assets/shaders/cubemap.frag");

  addShader("normal", normalShader);
  addShader("screen", screenShader);
  addShader("model", modelShader);
  addShader("cubemap", cubemapShader);
}

void EngineContext::setupModels(){
  auto tileModel = Model("../assets/models/tile.glb");
  auto diceModel = Model("../assets/models/dice.glb");

  addModel("tile", tileModel);
  addModel("dice", diceModel);
}

void EngineContext::setup(engineContext::SetupConfig config){
  input.setup();
  setupConfig(config);
  setupFramebuffers(config);
  setupGeometries();
  setupTextures();
  setupShaders();
  setupModels();
}
