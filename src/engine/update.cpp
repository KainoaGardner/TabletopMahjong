#include "../include/engine/update.hpp"
#include "../include/engine/engineContext.hpp"

#include <GLES3/gl3.h>

#include "../include/game/game.hpp"
#include "../include/engine/engineContext.hpp"
#include "../include/game/camera.hpp"

#include <iostream>

#include "../include/engine/collision.hpp"

namespace update {

void gameUpdate(EngineContext& engineCTX, Game& gameCTX){
  gameCTX.gameUpdate(engineCTX.input);

  gameCTX.lastGameState.createGameState(gameCTX);
}

void update(EngineContext& engineCTX, Game& gameCTX){
  engineCTX.input.update();

  if (engineCTX.input.justPressed(input::actions::click)){
    selectTile(engineCTX, gameCTX);
  }

  gameCTX.update(engineCTX.input);

  engineCTX.input.actionPrev = engineCTX.input.actionCurr;
  engineCTX.input.clear();
}

void selectTile(EngineContext& engineCTX, Game& gameCTX){
    const std::unique_ptr<Camera>& camera = gameCTX.cameras[gameCTX.currCamera];
    const glm::mat4& view = camera::getViewMatrix(camera->position, camera->yaw, camera->pitch, camera->roll);
    const glm::mat4& projection = camera::getProjectionMatrix(camera->fov, engineCTX.width, engineCTX.height);

    glm::vec3 rayDir;
    glm::vec3 rayOrigin;
    collision::computeMouseRay(engineCTX.input.mouse.x, engineCTX.input.mouse.y, engineCTX.width, engineCTX.height,
                               view, projection, camera->position,
                               rayOrigin, rayDir);

    std::cout << rayDir.x << " " << rayDir.y << std::endl;

}

}
