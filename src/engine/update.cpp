#include "../include/engine/update.hpp"
#include "../include/engine/engineContext.hpp"

#include <GLES3/gl3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp> 

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
  // gameCTX.gameUpdate(engineCTX.input);
  // gameCTX.lastGameState.createGameState(gameCTX);
}

void update(EngineContext& engineCTX, Game& gameCTX){
  engineCTX.input.update();

  const std::unique_ptr<Camera>& camera = gameCTX.cameras[gameCTX.currCamera];
  const glm::mat4& view = camera::getViewMatrix(camera->position, camera->yaw, camera->pitch, camera->roll);
  const glm::mat4& projection = camera::getProjectionMatrix(camera->fov, engineCTX.width, engineCTX.height);

  const glm::mat4 inverseView = glm::inverse(view);
  const glm::mat4 inverseProjection = glm::inverse(projection);

  glm::vec3 rayDir;
  glm::vec3 rayOrigin;
  collision::computeMouseRay(engineCTX.input.mouse.x, engineCTX.input.mouse.y, engineCTX.width, engineCTX.height,
                             inverseView, inverseProjection, camera->position,
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

  mouse(engineCTX, gameCTX, inverseView, inverseProjection, rayDir, rayOrigin, player);

  tileRotate(engineCTX, gameCTX, player);

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

bool selectionBoxSelectTile(EngineContext& engineCTX, Game& gameCTX,
                            const glm::mat4& inverseView, const glm::mat4& inverseProjection,
                            glm::vec2 startPos, glm::vec2 endPos, global::players player){

  std::array<glm::vec3, 8> points = collision::createFrustumPoints(inverseView, inverseProjection, startPos, endPos, engineCTX.width, engineCTX.height);
  std::cout << "start" << std::endl;
  for (int i = 0; i < 8; ++i){
    std::cout << i << std::endl;
    glm::vec3 p = points[i];
    engineCTX.testPoints[i] = p;
    std::cout << p.x << " " << p.y << " " << p.z << std::endl;
  }
  return collision::selectionBoxPickTile(points, gameCTX.tiles, player);
}

void click(EngineContext& engineCTX, Game& gameCTX, glm::vec3 rayDir, glm::vec3 rayOrigin, global::players player){
  if (!engineCTX.input.justPressed(input::actions::click))
    return;

  engineCTX.input.mouse.mouseDownPos = glm::vec2(engineCTX.input.mouse.x, engineCTX.input.mouse.y);
  engineCTX.input.mouse.mouseDownTime = std::chrono::steady_clock::now();
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

  auto now = std::chrono::steady_clock::now();
  auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(now - engineCTX.input.mouse.mouseDownTime).count();

  if (!engineCTX.input.mouse.drag && !engineCTX.input.mouse.selection){
    if (dist > global::dragDistThreshold || timePassed > global::dragTimeThreshold){
      if (engineCTX.input.mouse.tileClicked){
        engineCTX.input.mouse.drag = true;
      }else{
        engineCTX.input.mouse.selection = true;
      }
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

void release(EngineContext& engineCTX, Game& gameCTX, const glm::mat4& inverseView, const glm::mat4& inverseProjection, global::players player){
  if (!engineCTX.input.justReleased(input::actions::click))
    return;


  if (!engineCTX.input.mouse.drag &&
    engineCTX.input.mouse.tileClicked &&
    engineCTX.input.mouse.reselectTile &&
    engineCTX.input.pressed(input::actions::shift)){
    engineCTX.input.mouse.tileClicked->selected = std::nullopt;
  }

  if (engineCTX.input.mouse.selection){
    glm::vec2 currMousePos = glm::vec2(engineCTX.input.mouse.x, engineCTX.input.mouse.y);
    bool selected = selectionBoxSelectTile(engineCTX, gameCTX,inverseView, inverseProjection, engineCTX.input.mouse.mouseDownPos, currMousePos, player);
  }

  engineCTX.input.mouse.drag = false;
  engineCTX.input.mouse.selection = false;
  engineCTX.input.mouse.reselectTile = false;
  engineCTX.input.mouse.tileClicked = nullptr;
}


void mouse(EngineContext& engineCTX, Game& gameCTX,
             const glm::mat4& inverseView, const glm::mat4& inverseProjection,
             glm::vec3 rayDir, glm::vec3 rayOrigin, global::players player){
  click(engineCTX, gameCTX, rayDir, rayOrigin, player);
  hold(engineCTX, gameCTX, rayDir, rayOrigin, player);
  release(engineCTX, gameCTX, inverseView, inverseProjection, player);
}

void tileRotate(EngineContext& engineCTX, Game& gameCTX, global::players player){
  glm::vec3 axis(0.0f);
  float angle = glm::radians(-90.0f);

  if (engineCTX.input.justPressed(input::actions::spinForward))
    axis.x = -1.0;
  else if (engineCTX.input.justPressed(input::actions::spinBackward))
    axis.x = 1.0;
  else if (engineCTX.input.justPressed(input::actions::twistForward))
    axis.y = -1.0;
  else if (engineCTX.input.justPressed(input::actions::twistBackward))
    axis.y = 1.0;
  else if (engineCTX.input.justPressed(input::actions::flipForward))
    axis.z = -1.0;
  else if (engineCTX.input.justPressed(input::actions::flipBackward))
    axis.z = 1.0;


  if (axis.x == 0.0f && axis.y == 0.0f && axis.z == 0.0f){
    return;
  }

  axis = glm::rotate(axis, glm::radians(90.0f * player), glm::vec3(0.0f, 1.0f, 0.0f));

  for (const auto& tile : gameCTX.tiles){
    if (!tile->selected || tile->selected != player){
      continue;
    }

    glm::quat rotation = glm::angleAxis(angle, axis);
    tile->orientation = rotation * tile->orientation;
  } 
}

}


