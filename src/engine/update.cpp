#include "../include/engine/update.hpp"
#include "../include/engine/engineContext.hpp"

#include <GLES3/gl3.h>

#include "../include/engine/config.hpp"
#include "../include/game/game.hpp"
#include "../include/engine/engineContext.hpp"
#include "../include/game/camera.hpp"
#include "../include/game/tile.hpp"
#include "../include/game/hand.hpp"

#include "../include/engine/collision.hpp"

#include <iostream>

namespace update {

void gameUpdate(EngineContext& engineCTX, Game& gameCTX){
  gameCTX.gameUpdate(engineCTX.input);
  gameCTX.lastGameState.createGameState(gameCTX);
}

void update(EngineContext& engineCTX, Game& gameCTX){
  engineCTX.input.update();


  const std::unique_ptr<Camera>& camera = gameCTX.cameras[gameCTX.currCamera];
  const glm::mat4& view = camera::getViewMatrix(camera->position, camera->yaw, camera->pitch, camera->roll);
  const glm::mat4& projection = camera::getProjectionMatrix(camera->fov, engineCTX.width, engineCTX.height);

  glm::vec3 rayDir;
  glm::vec3 rayOrigin;
  collision::computeMouseRay(engineCTX.input.mouse.x, engineCTX.input.mouse.y, engineCTX.width, engineCTX.height,
                             view, projection, camera->position,
                             rayOrigin, rayDir);

  global::players player = global::players::jicha;

  //REMOVE TESTING
  if (gameCTX.currCamera == camera::nan1){
    player = global::players::shimocha;
  }else if (gameCTX.currCamera == camera::sha1){
    player = global::players::toimen;
  }else if (gameCTX.currCamera == camera::pei1){
    player = global::players::kamicha;
  }

  mouse(engineCTX, gameCTX, rayDir, rayOrigin, player);
  updateHands(gameCTX, rayDir, rayOrigin, player);

  gameCTX.update(engineCTX.input);

  engineCTX.input.actionPrev = engineCTX.input.actionCurr;
  engineCTX.input.clear();
}

void updateHands(Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin, global::players player){
  gameCTX.hands[player]->update(rayDir, rayOrigin);
}


Tile* selectTile(EngineContext& engineCTX, Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin, bool& reselect, global::players player){

  Tile* tile = collision::pickTile(rayOrigin, rayDir, gameCTX.tiles);
  if (tile != nullptr){
    if (tile->selected == player){
      // tile->selected = std::nullopt;
      reselect = true;
      return tile;
    }


    tile->selected = player;
    return tile;
  }

  return nullptr;
}

void unselectPlayerTiles(global::players player, Game& gameCTX){
  for (const auto& tile : gameCTX.tiles){
    if (!tile->selected)
      continue;

    if (tile->selected == player){
      tile->selected = std::nullopt;
    }
  };
}

void click(EngineContext& engineCTX, Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin, global::players player){
  if (!engineCTX.input.justPressed(input::actions::click))
    return;

  engineCTX.input.mouse.mouseDownPos = glm::vec2(engineCTX.input.mouse.x, engineCTX.input.mouse.y);
  engineCTX.input.mouse.drag = false;
  engineCTX.input.mouse.selection = false;
  bool reselect = false;
  engineCTX.input.mouse.tileClicked = selectTile(engineCTX, gameCTX, rayDir, rayOrigin, reselect, player);
  engineCTX.input.mouse.reselectTile = reselect;

  if (!engineCTX.input.pressed(input::actions::shift)){
    unselectPlayerTiles(player, gameCTX);
    if (engineCTX.input.mouse.tileClicked != nullptr){
      engineCTX.input.mouse.tileClicked->selected = player;
    }
  }
}

void hold(EngineContext& engineCTX, Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin, global::players player){
  if (!engineCTX.input.pressed(input::actions::click))
    return;

  glm::vec2 currMousePos = glm::vec2(engineCTX.input.mouse.x, engineCTX.input.mouse.y);
  float dist = glm::distance(engineCTX.input.mouse.mouseDownPos, currMousePos);

  if ((!engineCTX.input.mouse.drag && !engineCTX.input.mouse.selection) && dist > global::dragThreshold){
    if (engineCTX.input.mouse.tileClicked){
      engineCTX.input.mouse.drag = true;
    }else{
      engineCTX.input.mouse.selection = true;
    }
  }

  //drag
  if (engineCTX.input.mouse.drag){
    float t = -rayOrigin.y / rayDir.y;
    glm::vec3 mousePos = rayOrigin + t * rayDir;

    //change to mousedown pos
    glm::vec3 clickPos = engineCTX.input.mouse.tileClicked->position;

    for (const auto& tile : gameCTX.tiles){
      if (tile->selected == player){
        glm::vec3 posDelta = mousePos - clickPos;
        posDelta.y = mousePos.y;
        tile->position += posDelta;
      }
    }

  }

}

void release(EngineContext& engineCTX, Game& gameCTX, global::players player){
  if (!engineCTX.input.justReleased(input::actions::click))
    return;

  engineCTX.input.mouse.drag = false;
  engineCTX.input.mouse.selection = false;
  engineCTX.input.mouse.reselectTile = false;
  engineCTX.input.mouse.tileClicked = nullptr;
}


void mouse(EngineContext& engineCTX, Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin, global::players player){
  click(engineCTX, gameCTX, rayDir, rayOrigin, player);
  hold(engineCTX, gameCTX, rayDir, rayOrigin, player);
  release(engineCTX, gameCTX, player);
}


}


