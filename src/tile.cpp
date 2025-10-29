#include "../include/tile.hpp"
#include "../include/model.hpp"
#include "../include/shader.hpp"
#include <iostream>

namespace tile {
std::unordered_map<int, glm::vec2> tileUV = {
  {Man1, glm::vec2(1,0)},
  {Man2, glm::vec2(1,1)},
  {Man3, glm::vec2(1,2)},
  {Man4, glm::vec2(1,3)},
  {Man5, glm::vec2(1,4)},
  {Man6, glm::vec2(1,5)},
  {Man7, glm::vec2(1,6)},
  {Man8, glm::vec2(1,7)},
  {Man9, glm::vec2(1,8)},
  {Sou1, glm::vec2(2,0)},
  {Sou2, glm::vec2(2,1)},
  {Sou3, glm::vec2(2,2)},
  {Sou4, glm::vec2(2,3)},
  {Sou5, glm::vec2(2,5)},
  {Sou6, glm::vec2(2,6)},
  {Sou7, glm::vec2(2,7)},
  {Sou8, glm::vec2(2,8)},
  {Sou9, glm::vec2(2,9)},
  {Pin1, glm::vec2(3,0)},
  {Pin2, glm::vec2(3,1)},
  {Pin3, glm::vec2(3,2)},
  {Pin4, glm::vec2(3,3)},
  {Pin5, glm::vec2(3,5)},
  {Pin6, glm::vec2(3,6)},
  {Pin7, glm::vec2(3,7)},
  {Pin8, glm::vec2(3,8)},
  {Pin9, glm::vec2(3,9)},
  {Ton , glm::vec2(0,0)},
  {Nan , glm::vec2(0,1)},
  {Sha , glm::vec2(0,2)},
  {Pei , glm::vec2(0,3)},
  {Haku, glm::vec2(0,6)},
  {Hatsu, glm::vec2(0,4)},
  {Chun, glm::vec2(0,5)},
  {Man5A, glm::vec2(1,9)},
  {Sou5A, glm::vec2(2,4)},
  {Pin5A, glm::vec2(3,4)},
};

}

Tile::Tile(unsigned int tileIn, const Model* modelIn){
  tile = tileIn;
  model = modelIn;
}

void Tile::draw() const {
  if (model == nullptr) return;

  shader::shader.model->use();

  for (int i = 0; i < model->meshes.size(); i++){
    const model::Mesh& mesh = model->meshes[i];
    glBindTexture(GL_TEXTURE_2D, mesh.textureIndex);
    glBindVertexArray(mesh.vao);
    shader::shader.model->setInt("uDiffuse0",0);

    if (i == 1) {
      auto it = tile::tileUV.find(tile);
      glm::vec2 uvPos = glm::vec2(0.0f);
      if (it != tile::tileUV.end()){
        uvPos = it->second;

        glm::vec2 tileMapRatio = glm::vec2(model::tileScale.z / model::tileScale.x * 0.10075 ,0.1f);
        // uvPos *= model::tileMapRatio;
        uvPos *= tileMapRatio;
      }
      shader::shader.model->setVec2f("uTexOffset", uvPos);
    }else {
      shader::shader.model->setVec2f("uTexOffset", glm::vec2(0.0f));
    }

    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_SHORT, 0);
  }

  glBindVertexArray(0);
}

namespace tile {
std::vector<Tile> tiles;

void setup(int type){
  switch(type){
    default:
      for (int i = 0; i < 37; ++i){
        tiles.emplace_back(i, model::model.tile.get());
      }

      // tiles.emplace_back(Chun, model::model.tile.get());
    break;
  }

}

}
