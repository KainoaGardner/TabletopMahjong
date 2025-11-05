#include <GLES3/gl3.h>

#include "../include/render.hpp"
#include "../include/shader.hpp"
#include "../include/geometry.hpp"
#include "../include/framebuffer.hpp"
#include "../include/texture.hpp"
#include "../include/model.hpp"
#include "../include/config.hpp"
#include "../include/camera.hpp"
#include "../include/tile.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>


namespace render {
void main(){
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer::framebuffers.screen.fbo);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  std::unique_ptr<Camera>& camera = camera::cameras[camera::curr];
  glm::mat4 view = camera->getViewMatrix();
  glm::mat4 projection = camera->getProjectionMatrix();

  cubemap(view, projection);

  mat(view,projection);
  table(view,projection);
  
  tiles(view, projection);
  dice(view, projection);

  screen();
}

void tiles(const glm::mat4& view, const glm::mat4& projection){
  int i = 0;
  for (const Tile& tile : tile::tiles){
    shader::shader.model->use();
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 tileScale = glm::vec3(1.14);
    model = glm::translate(model, tile.position);
    model *= glm::mat4_cast(tile.orientation);
    model = glm::scale(model, tileScale);
    shader::shader.model->setMatrix4fv("uModel", model);
    shader::shader.model->setMatrix4fv("uView", view);
    shader::shader.model->setMatrix4fv("uProjection", projection);
    tile.draw();
  }

}

void dice(const glm::mat4& view, const glm::mat4& projection){
  shader::shader.model->use();
  glm::mat4 model = glm::mat4(1.0f);

  model = glm::translate(model, glm::vec3(0.0f, model::diceScale.y / 2.0f,0.0f));
  shader::shader.model->setMatrix4fv("uModel", model);
  model::model.dice->draw();
}

void mat(const glm::mat4& view, const glm::mat4& projection){
  shader::shader.normal->use();
  glm::mat4 model = glm::mat4(1.0f);

  glBindVertexArray(geometry::geometry.plane.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry::geometry.plane.ebo);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture::texture.mat.texture);
  shader::shader.normal->setVec2f("uTexScale", glm::vec2(1.0f));
  shader::shader.normal->setInt("uDiff",0);

  model = glm::mat4(1.0f);
  model = glm::scale(model, model::matScale);
  model = glm::translate(model, glm::vec3(0.0f,-0.5f,0.0f));
  model = glm::rotate(model,glm::radians(90.0f),global::worldRight);

  shader::shader.normal->setMatrix4fv("uModel", model);
  shader::shader.normal->setMatrix4fv("uView", view);
  shader::shader.normal->setMatrix4fv("uProjection", projection);


  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void table(const glm::mat4& view, const glm::mat4& projection){
  shader::shader.normal->use();
  glm::mat4 model = glm::mat4(1.0f);

  glBindVertexArray(geometry::geometry.cube.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry::geometry.cube.ebo);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture::texture.tableSide.texture);
  shader::shader.normal->setVec2f("uTexScale", glm::vec2(1.0f, 0.05f));
  shader::shader.normal->setInt("uDiff",0);
  shader::shader.normal->setMatrix4fv("uView", view);
  shader::shader.normal->setMatrix4fv("uProjection", projection);


  float offsetDiff = -model::matScale.z / 2.0 - model::tableSideLongScale.z / 2.0;
  for (int i = 0; i < 2; ++i){
    model = glm::mat4(1.0f);
    model = glm::rotate(model,glm::radians(90.0f) * (i * 2),global::worldUp);
    model = glm::translate(model, glm::vec3(0.0f,0.0f, offsetDiff));
    model = glm::scale(model, model::tableSideLongScale);

    shader::shader.normal->setMatrix4fv("uModel", model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }

  for (int i = 0; i < 2; ++i){
    model = glm::mat4(1.0f);
    model = glm::rotate(model,glm::radians(90.0f) * (i * 2) + glm::radians(90.0f),global::worldUp);
    model = glm::translate(model, glm::vec3(0.0f,0.0f, offsetDiff));
    model = glm::scale(model, model::tableSideShortScale);

    shader::shader.normal->setMatrix4fv("uModel", model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }
}

void cubemap(const glm::mat4& view, const glm::mat4& projection){
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_FALSE);

  glBindVertexArray(geometry::geometry.cube.vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture::texture.room1.texture);

  shader::shader.cubemap->use();
  shader::shader.cubemap->setMatrix4fv("uProjection", projection);
  glm::mat4 noTranslateView = glm::mat4(glm::mat3(view));

  shader::shader.cubemap->setMatrix4fv("uView", noTranslateView);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);

}

void screen(){
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader::shader.screen->use();
  glBindVertexArray(geometry::geometry.screen.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry::geometry.screen.ebo);
  glDisable(GL_DEPTH_TEST);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, framebuffer::framebuffers.screen.texture);
  shader::shader.screen->setInt("uTexture0",0);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
}
