#include <GLES3/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "../include/engine/render.hpp"
#include "../include/engine/config.hpp"
#include "../include/game/game.hpp"
#include "../include/game/hand.hpp"
#include "../include/engine/model.hpp"
#include "../include/game/tile.hpp"
#include "../include/engine/shader.hpp"
#include "../include/engine/engineContext.hpp"
#include "../include/game/camera.hpp"
#include "../include/engine/collision.hpp"
#include "../include/game/gameState.hpp"

#include <iostream>

namespace render {
glm::vec3 linearInterp(const glm::vec3& a, const glm::vec3& b, float alpha){
  return a + alpha * (b - a);
}

float linearInterp(float a, float b, float alpha){
  return a + alpha * (b - a);
}

glm::mat4 getInterpCameraView(float a, Game& gameCTX){
  const std::unique_ptr<Camera>& camera = gameCTX.cameras[gameCTX.currCamera];
  gameState::CameraState& cameraState = gameCTX.lastGameState.cameras[gameCTX.currCamera];

  glm::vec3 positionState;
  float yawState;
  float pitchState;
  float rollState;
  if (gameCTX.lastGameState.firstFrame){
    positionState = camera->position;
    yawState = camera->yaw;
    pitchState = camera->pitch;
    rollState = camera->roll;
  }else {
    positionState = cameraState.position;
    yawState = cameraState.yaw;
    pitchState = cameraState.pitch;
    rollState = cameraState.roll;
  }

  glm::vec3 positionInterp = linearInterp(camera->position, positionState, a);
  float yawInterp = linearInterp(camera->yaw, yawState, a);
  float pitchInterp = linearInterp(camera->pitch, pitchState, a);
  float rollInterp = linearInterp(camera->roll, rollState, a);

  glm::mat4 view = camera::getViewMatrix(positionInterp, yawInterp, pitchInterp, rollInterp);
  return view;
}

glm::mat4 getInterpCameraProj(float a, Game& gameCTX, EngineContext& engineCTX){
  const std::unique_ptr<Camera>& camera = gameCTX.cameras[gameCTX.currCamera];
  gameState::CameraState& cameraState = gameCTX.lastGameState.cameras[gameCTX.currCamera];

  float fovState;
  if (gameCTX.lastGameState.firstFrame){
    fovState = camera->fov;
  }else {
    fovState = cameraState.fov;
  }

  float fovInterp  = linearInterp(camera->fov, fovState, a);

  glm::mat4 projection = camera::getProjectionMatrix(fovInterp, engineCTX.width, engineCTX.height);
  return projection;
}


void main(float a, EngineContext& engineCTX, Game& gameCTX){
  glBindFramebuffer(GL_FRAMEBUFFER, engineCTX.getFramebuffer("screen")->fbo);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glStencilMask(0x00);

  glm::mat4 view = getInterpCameraView(a, gameCTX);
  glm::mat4 projection = getInterpCameraProj(a, gameCTX, engineCTX);

  cubemap(engineCTX, view, projection);

  mat(engineCTX, view, projection);
  table(engineCTX, view, projection);
  centerPiece(engineCTX, gameCTX, view, projection);
  
  tiles(engineCTX, gameCTX.tiles, view, projection);

  // dice(engineCTX, view, projection);

  selectHighlight(engineCTX, gameCTX.tiles, view, projection);
  // click(engineCTX, gameCTX, view, projection);
  hands(engineCTX, gameCTX, view, projection);
  screen(engineCTX);
}

void tiles(EngineContext& engineCTX, const std::vector<std::unique_ptr<Tile>>& tiles, const glm::mat4& view, const glm::mat4& projection){
  Shader* shader = engineCTX.getShader("model");

  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  for (const auto& tile : tiles){
    if (tile->selected){
      glStencilMask(0xFF);
    }else{
      glStencilMask(0x00);
    }

    shader->use();
    shader->setMatrix4fv("uView", view);
    shader->setMatrix4fv("uProjection", projection);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, tile->position);
    model *= glm::mat4_cast(tile->orientation);
    model = glm::scale(model, glm::vec3(model::tileScaleFactor));
    shader->setMatrix4fv("uModel", model);
    tile->draw(shader);
  }

  glStencilMask(0x00);
}

void selectHighlight(EngineContext& engineCTX, const std::vector<std::unique_ptr<Tile>>& tiles, const glm::mat4& view, const glm::mat4& projection){
  Shader* outlineShader = engineCTX.getShader("click");
  Geometry* cubeGeo = engineCTX.getGeometry("cube");

  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  glStencilMask(0x00);
  glDisable(GL_DEPTH_TEST);

  for (const auto& tile : tiles){
    if (!tile->selected)
      continue;

    outlineShader->use();
    outlineShader->setMatrix4fv("uView", view);
    outlineShader->setMatrix4fv("uProjection", projection);
  
    glBindVertexArray(cubeGeo->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeGeo->ebo);
  
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, tile->position);
    model *= glm::mat4_cast(tile->orientation);
    model = glm::scale(model, glm::vec3(tile->scale * 1.2f));

    //default color
    // glm::vec3 color = glm::vec3(1.0, 1.0, 0.0);
    glm::vec3 color = game::getPlayerColor(*tile->selected);

    //get color based on player color
    // auto it = global::colorToVec3.find(global::colors::red);
    // if (it != global::colorToVec3.end()){
    //   color = it->second;
    // }

    outlineShader->setVec3f("uColor", color);

    outlineShader->setMatrix4fv("uModel", model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }

  glStencilMask(0xFF);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glEnable(GL_DEPTH_TEST);
}


void dice(EngineContext& engineCTX, const glm::mat4& view, const glm::mat4& projection){
  Shader* shader = engineCTX.getShader("model");
  Model* diceModel = engineCTX.getModel("dice");

  shader->use();

  shader->setMatrix4fv("uView", view);
  shader->setMatrix4fv("uProjection", projection);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, model::diceScale.y / 2.0f,0.0f));
  shader->setMatrix4fv("uModel", model);

  diceModel->draw(shader);
}

void mat(EngineContext& engineCTX, const glm::mat4& view, const glm::mat4& projection){
  Shader* shader = engineCTX.getShader("normal");
  Geometry* planeGeo = engineCTX.getGeometry("plane");
  Texture* matTexture = engineCTX.getTexture("mat");

  shader->use();
  glm::mat4 model = glm::mat4(1.0f);

  glBindVertexArray(planeGeo->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeGeo->ebo);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, matTexture->texture);
  shader->setVec2f("uTexScale", glm::vec2(1.0f));
  shader->setInt("uDiff",0);

  model = glm::mat4(1.0f);
  model = glm::scale(model, model::matScale);
  model = glm::translate(model, glm::vec3(0.0f,-0.5f,0.0f));
  model = glm::rotate(model,glm::radians(90.0f), global::worldRight);

  shader->setMatrix4fv("uModel", model);
  shader->setMatrix4fv("uView", view);
  shader->setMatrix4fv("uProjection", projection);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void table(EngineContext& engineCTX, const glm::mat4& view, const glm::mat4& projection){
  Shader* shader = engineCTX.getShader("normal");
  Geometry* cubeGeo = engineCTX.getGeometry("cube");
  Texture* tableTexture = engineCTX.getTexture("tableSide");

  shader->use();
  glm::mat4 model = glm::mat4(1.0f);

  glBindVertexArray(cubeGeo->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeGeo->ebo);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tableTexture->texture);
  shader->setVec2f("uTexScale", glm::vec2(1.0f, 0.05f));
  shader->setInt("uDiff",0);
  shader->setMatrix4fv("uView", view);
  shader->setMatrix4fv("uProjection", projection);

  float offsetDiff = -model::matScale.z / 2.0 - model::tableSideLongScale.z / 2.0;
  for (int i = 0; i < 4; ++i){
    model = glm::mat4(1.0f);
    model = glm::rotate(model,glm::radians(90.0f) * i,global::worldUp);
    model = glm::translate(model, glm::vec3(0.0f,0.0f, offsetDiff));
    model = glm::scale(model, model::tableSideShortScale);

    shader->setMatrix4fv("uModel", model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }

  // float cornerOffsetDiffVert = -model::tableSideShortScale.x / 2.0 - model::tableSideLongScale.z / 2.0;
  float cornerOffsetDiffVert = -model::tableSideShortScale.x / 2.0 + model::tableSideLongScale.z / 2.0;
  for (int i = 0; i < 4; ++i){
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f) * i, global::worldUp);
    model = glm::translate(model, glm::vec3(offsetDiff, cornerOffsetDiffVert, offsetDiff));
    model = glm::rotate(model, glm::radians(90.0f), global::worldFront);
    model = glm::scale(model, model::tableSideShortScale);

    shader->setMatrix4fv("uModel", model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }

}

void cubemap(EngineContext& engineCTX, const glm::mat4& view, const glm::mat4& projection){
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_FALSE);

  glBindVertexArray(engineCTX.getGeometry("cube")->vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, engineCTX.getTexture("sky4")->texture);

  Shader* shader = engineCTX.getShader("cubemap");
  
  shader->use();
  shader->setMatrix4fv("uProjection", projection);
  glm::mat4 noTranslateView = glm::mat4(glm::mat3(view));

  shader->setMatrix4fv("uView", noTranslateView);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);

}

void click(EngineContext& engineCTX, Game& gameCTX, const glm::mat4& view, const glm::mat4& projection){
  glEnable(GL_BLEND);

  Shader* shader = engineCTX.getShader("click");
  Geometry* cubeGeo = engineCTX.getGeometry("cube");

  shader->use();
  glm::mat4 model = glm::mat4(1.0f);

  glBindVertexArray(cubeGeo->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeGeo->ebo);

  shader->setMatrix4fv("uView", view);
  shader->setMatrix4fv("uProjection", projection);

  const std::unique_ptr<Camera>& camera = gameCTX.cameras[gameCTX.currCamera];
  glm::vec3 rayDir;
  glm::vec3 rayOrigin;
  collision::computeMouseRay(engineCTX.input.mouse.x, engineCTX.input.mouse.y, engineCTX.width, engineCTX.height,
                             view, projection, camera->position,
                             rayOrigin, rayDir);

  float t = -rayOrigin.y / rayDir.y;
  glm::vec3 clickPos = rayOrigin + t * rayDir;
  // gameCTX.click = clickPos;


  model = glm::mat4(1.0f);
  model = glm::translate(model, clickPos);
  model = glm::scale(model, glm::vec3(0.015f));

    //default color

  global::players player;
  switch(gameCTX.currCamera){
    case camera::ton1:
      player = global::players::jicha;
    break;
    case camera::nan1:
      player = global::players::shimocha;
    break;
    case camera::sha1:
      player = global::players::toimen;
    break;
    case camera::pei1:
      player = global::players::kamicha;
    break;
  }

  glm::vec3 color = game::getPlayerColor(player);



  // //get color based on player color
  // auto it = global::colorToVec3.find(global::colors::red);
  // if (it != global::colorToVec3.end()){
  //   color = it->second;
  // }

  shader->setVec3f("uColor", color);
  shader->setMatrix4fv("uModel", model);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


  glDisable(GL_BLEND);
}

void hands(EngineContext& engineCTX, Game& gameCTX, const glm::mat4& view, const glm::mat4& projection){
  glEnable(GL_BLEND);

  Shader* shader = engineCTX.getShader("click");
  Geometry* cubeGeo = engineCTX.getGeometry("cube");

  shader->use();

  glBindVertexArray(cubeGeo->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeGeo->ebo);

  shader->setMatrix4fv("uView", view);
  shader->setMatrix4fv("uProjection", projection);

  for (const auto& hand : gameCTX.hands){
    glm::mat4 model = glm::mat4(1.0f);
  
    model = glm::mat4(1.0f);
    model = glm::translate(model, hand->position);
    model = glm::scale(model, glm::vec3(0.015f));
  
    shader->setVec3f("uColor", hand->color);
    shader->setMatrix4fv("uModel", model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }


  glDisable(GL_BLEND);
}

void centerPiece(EngineContext& engineCTX, Game& gameCTX, const glm::mat4& view, const glm::mat4& projection){
  Shader* shader = engineCTX.getShader("normal");
  Geometry* cubeGeo = engineCTX.getGeometry("cube");
  Texture* tableTexture = engineCTX.getTexture("tableSide");

  shader->use();
  glm::mat4 model = glm::mat4(1.0f);

  glBindVertexArray(cubeGeo->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeGeo->ebo);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tableTexture->texture);
  shader->setVec2f("uTexScale", glm::vec2(1.0f, 0.05f));
  shader->setInt("uDiff",0);
  shader->setMatrix4fv("uView", view);
  shader->setMatrix4fv("uProjection", projection);

  model = glm::mat4(1.0f);
  model = glm::scale(model, model::centerPieceScale);

  shader->setMatrix4fv("uModel", model);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void screen(EngineContext& engineCTX){
  Shader* shader = engineCTX.getShader("screen");
  Geometry* screenGeo = engineCTX.getGeometry("screen");
  Framebuffer* screenFramebuffer = engineCTX.getFramebuffer("screen");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader->use();
  glBindVertexArray(screenGeo->vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenGeo->ebo);
  glDisable(GL_DEPTH_TEST);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, screenFramebuffer->texture);
  shader->setInt("uTexture0",0);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
}
