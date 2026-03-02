#include "engine/engineContext.hpp"
#include "engine/model.hpp"
#include "engine/shader.hpp"

EngineContext::EngineContext(){
}

EngineContext::~EngineContext(){
}

//Framebuffers
Framebuffer* EngineContext::addFrameBuffer(const std::string& name, std::unique_ptr<Framebuffer> framebuffer){
  framebuffers[name] = std::move(framebuffer);
  return framebuffers[name].get();
}

Framebuffer* EngineContext::getFramebuffer(const std::string& name){
  return framebuffers.at(name).get();
}

//Geometries
Geometry* EngineContext::addGeometry(const std::string& name, std::unique_ptr<Geometry> geometry){
  geometries[name] = std::move(geometry);
  return geometries[name].get();
}

Geometry* EngineContext::getGeometry(const std::string& name){
  return geometries.at(name).get();
}

//Textures
Texture* EngineContext::addTexture(const std::string& name, std::unique_ptr<Texture> texture){
  textures[name] = std::move(texture);
  return textures[name].get();
}

Texture* EngineContext::getTexture(const std::string& name){
  return textures.at(name).get();
}

//Shaders
Shader* EngineContext::addShader(const std::string& name, std::unique_ptr<Shader> shader){
  shaders[name] = std::move(shader);
  return shaders[name].get();
}

Shader* EngineContext::getShader(const std::string& name){
  return shaders.at(name).get();
}


//Models
Model* EngineContext::addModel(const std::string& name, std::unique_ptr<Model> model){
  models[name] = std::move(model);
  return models[name].get();
}

Model* EngineContext::getModel(const std::string& name){
  return models.at(name).get();
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

  addTexture("mat", std::make_unique<Texture>(std::move(matTexture)));
  addTexture("tableSide", std::make_unique<Texture>(std::move(woodTexture)));
  addTexture("sky4", std::make_unique<Texture>(std::move(sky4Texture)));
}

void EngineContext::setupFramebuffers(engineContext::SetupConfig config){
  auto screenFramebuffer = std::make_unique<Framebuffer>(framebuffer::create(config.width, config.height));
  auto highlightFramebuffer = std::make_unique<Framebuffer>(framebuffer::createColorBuffer(config.width, config.height, true));

  addFrameBuffer("screen", std::move(screenFramebuffer));
  addFrameBuffer("highlight", std::move(highlightFramebuffer));
}

void EngineContext::setupGeometries(){
  auto planeGeometry = std::make_unique<Geometry>(geometry::createPlane());
  auto cubeGeometry = std::make_unique<Geometry>(geometry::createCube());
  auto cubemapGeometry = std::make_unique<Geometry>(geometry::createCubemap());
  auto screenGeometry = std::make_unique<Geometry>(geometry::createScreen());
  auto quadGeometry = std::make_unique<Geometry>(geometry::createQuad());
  auto lineGeometry = std::make_unique<Geometry>(geometry::createLine());

  addGeometry("plane", std::move(planeGeometry));
  addGeometry("cube", std::move(cubeGeometry));
  addGeometry("cubemap", std::move(cubemapGeometry));
  addGeometry("screen", std::move(screenGeometry));
  addGeometry("quad", std::move(quadGeometry));
  addGeometry("line", std::move(lineGeometry));
}

void EngineContext::setupShaders(){
  auto normalShader = std::make_unique<Shader>("./assets/shaders/vertex.vert", "./assets/shaders/fragment.frag");
  auto clickShader = std::make_unique<Shader>("./assets/shaders/vertex.vert", "./assets/shaders/click.frag");
  auto blackShader = std::make_unique<Shader>("./assets/shaders/vertex.vert", "./assets/shaders/black.frag");
  auto screenShader = std::make_unique<Shader>("./assets/shaders/screen.vert", "./assets/shaders/screen.frag");
  auto modelShader =  std::make_unique<Shader>("./assets/shaders/model.vert", "./assets/shaders/model.frag");
  auto cubemapShader = std::make_unique<Shader>("./assets/shaders/cubemap.vert", "./assets/shaders/cubemap.frag");

  auto selectionBoxShader = std::make_unique<Shader>("./assets/shaders/selectionBox.vert", "./assets/shaders/selectionBox.frag");
  auto lineShader = std::make_unique<Shader>("./assets/shaders/line.vert", "./assets/shaders/line.frag");

  addShader("normal", std::move(normalShader));
  addShader("click", std::move(clickShader));
  addShader("black", std::move(blackShader));
  addShader("screen", std::move(screenShader));
  addShader("model", std::move(modelShader));
  addShader("cubemap", std::move(cubemapShader));

  addShader("selectionBox", std::move(selectionBoxShader));
  addShader("line", std::move(lineShader));
}

void EngineContext::setupModels(){
  auto tileModel = std::make_unique<Model>("./assets/models/tile.glb");
  auto diceModel = std::make_unique<Model>("./assets/models/dice.glb");

  addModel("tile", std::move(tileModel));
  addModel("dice", std::move(diceModel));
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
