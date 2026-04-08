#include "app.hpp"
#include "game/tile.hpp"

#include "engine/render.hpp"
#include "engine/update.hpp"


void App::update(){
  update::update(engineCTX, gameCTX);
}

void App::gameUpdate(){
  update::gameUpdate(engineCTX, gameCTX);
}

void App::render(float a){
  render::main(0, engineCTX, gameCTX);
}

void App::setup(){
  engineContext::SetupConfig engineSetupConfig = {
    // .width = 1920,
    // .height = 1080,
    // .width = 1600,
    // .height = 900,
    // .width = 1280,
    // .height = 720,
    // .width = 960,
    // .height = 540,
    .width = 1366,
    .height = 768,
    .fps = 60,
  };


  engineCTX.setup(engineSetupConfig);

  game::SetupConfig gameSetupConfig = {
    .oya = 0,
    .playerCount = 4,
    .seat = 0,
    .tileSet = tile::TileSet::FourP,
    .tileModel = engineCTX.getModel("tile"),
  };

  gameCTX.setup(gameSetupConfig);
}
