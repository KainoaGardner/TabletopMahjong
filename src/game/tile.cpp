#include "engine/model.hpp"
#include "engine/shader.hpp"

#include "game/tile.hpp"

#include <cmath>
#include <glm/ext/scalar_constants.hpp>
#include <random>
#include <algorithm>

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

Tile::Tile(unsigned int tileIn, const Model* modelIn, glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn){
  tile = tileIn;
  model = modelIn;
  position = positionIn;
  orientation = orientationIn;

  scale = scaleIn;
  halfSize = scaleIn * 0.5f;
}

void Tile::draw(Shader* shader) const {
  if (model == nullptr) return;

  for (int i = 0; i < model->meshes.size(); i++){
    const model::Mesh& mesh = model->meshes[i];
    glBindVertexArray(mesh.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.textureIndex);
    shader->setInt("uDiffuse0",0);

    if (i == 1) {
      auto it = tile::tileUV.find(tile);
      glm::vec2 uvPos = glm::vec2(0.0f);
      if (it != tile::tileUV.end()){
        uvPos = it->second;

        glm::vec2 tileMapRatio = glm::vec2(model::tileScale.z / model::tileScale.x * 0.101f, 0.1f);
        uvPos *= tileMapRatio;
      }
      shader->setVec2f("uTexOffset", uvPos);
    }else {
      shader->setVec2f("uTexOffset", glm::vec2(0.0f));
    }

    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_SHORT, 0);
  }

  glBindVertexArray(0);
}

glm::mat4 Tile::getModelMatrix() const{
  glm::mat4 model = glm::mat4(1.0f);

  model = glm::translate(model, position);
  model *= glm::mat4_cast(orientation);
  model = glm::scale(model, glm::vec3(model::tileScaleFactor));

  return model;
}

namespace tile {

void setup(const game::SetupConfig& config, std::vector<std::unique_ptr<Tile>>& tiles){
  switch(config.tileSet){
    case FourP:
      fourPSetup(tiles, config.tileModel);
      break;
    case ThreeP:
      threePSetup(tiles, config.tileModel);
      break;
    case 3:{
      glm::quat orientation = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      tiles.emplace_back(std::make_unique<Tile>(Sou5A, config.tileModel, glm::vec3(0.0f), orientation, model::tileScale));
      break;
    }

    default:
    break;
  }

  int roll = 1;
  makeWalls(tiles);
  dealHands(roll, config.oya, tiles);
  makeDeadWall(roll, config.oya, tiles);
}

void fourPSetup(std::vector<std::unique_ptr<Tile>>& tiles, const Model* tileModel){
  glm::vec3 startPos = glm::vec3(-model::matScale.x / 2.0f + model::tileScale.x / 2.0f,
                                 model::tileScale.y / 2.0f,
                                 -model::matScale.z / 2.0f + model::tileScale.z / 2.0f);

  glm::quat orientation = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  for (int i = Man1; i <= Chun; ++i){
    for (int j = 0; j < 4; ++j){
      int c = (i * 4 + j) % 17;
      int r = (i * 4 + j) / 17;

      glm::vec3 pos = glm::vec3(startPos.x + c * model::tileScale.x, startPos.y, startPos.z + r * model::tileScale.z);
      if (j == 3 && (i == Man5 || i == Sou5 || i == Pin5)){
        if (i == Man5){
          tiles.emplace_back(std::make_unique<Tile>(Man5A, tileModel, pos, orientation, model::tileScale));
        }else if (i == Sou5){
          tiles.emplace_back(std::make_unique<Tile>(Sou5A, tileModel, pos, orientation, model::tileScale));
        }else{
          tiles.emplace_back(std::make_unique<Tile>(Pin5A, tileModel, pos, orientation, model::tileScale));
        }
      }else{
        tiles.emplace_back(std::make_unique<Tile>(i, tileModel, pos, orientation, model::tileScale));
      }
    }
  }
}

void threePSetup(std::vector<std::unique_ptr<Tile>>& tiles, const Model* tileModel){
  glm::vec3 startPos = glm::vec3(-model::matScale.x / 2.0f + model::tileScale.x / 2.0f,
                                 model::tileScale.y / 2.0f,
                                 -model::matScale.z / 2.0f + model::tileScale.z / 2.0f);
  glm::quat orientation = glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  for (int i = 0; i < 4; ++i){
      int c = (i) % 18;
      int r = (i) / 18;
      glm::vec3 pos = glm::vec3(startPos.x + c * model::tileScale.x, startPos.y, startPos.z + r * model::tileScale.z);
      tiles.emplace_back(std::make_unique<Tile>(Man1, tileModel, pos, orientation, model::tileScale));
  }
  for (int i = 0; i < 4; ++i){
      int c = (4 + i) % 18;
      int r = (4 + i) / 18;
      glm::vec3 pos = glm::vec3(startPos.x + c * model::tileScale.x, startPos.y, startPos.z + r * model::tileScale.z);
      tiles.emplace_back(std::make_unique<Tile>(Man9, tileModel, pos, orientation, model::tileScale));
  }

  for (int i = Sou1; i <= Chun; ++i){
    for (int j = 0; j < 4; ++j){
      int c = ((i) * 4 + j - 28) % 18;
      int r = ((i) * 4 + j - 28) / 18;

      glm::vec3 pos = glm::vec3(startPos.x + c * model::tileScale.x, startPos.y, startPos.z + r * model::tileScale.z);

      if (j == 3 && (i == Sou5 || i == Pin5)){
        if (i == Sou5){
          tiles.emplace_back(std::make_unique<Tile>(Sou5A, tileModel, pos, orientation, model::tileScale));
        }else{
          tiles.emplace_back(std::make_unique<Tile>(Pin5A, tileModel, pos, orientation, model::tileScale));
        }
      }else{
        tiles.emplace_back(std::make_unique<Tile>(i, tileModel, pos, orientation, model::tileScale));
      }
    }
  }
}


void shuffleTiles(std::vector<std::unique_ptr<Tile>>& tiles){
  std::random_device rand;
  std::default_random_engine gen(rand());
  std::shuffle(tiles.begin(), tiles.end(), gen);
}

void flipTiles(std::vector<std::unique_ptr<Tile>>& tiles) {
  glm::quat orientation = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  for (std::unique_ptr<Tile>& tile : tiles){
    tile->orientation = orientation;
  }
}

void makeWalls(std::vector<std::unique_ptr<Tile>>& tiles) {
  flipTiles(tiles);
  shuffleTiles(tiles);

  int tileWidth;
  if (tiles.size() == 108){
    tileWidth = 18;
  }else {
    tileWidth = 17;
  }


  float r = model::wallRadiusDistance;
  float startY = model::tileScale.y / 2.0f;
  float offset = -model::tileScale.x * (tileWidth / 2.0f - 0.5f);

  int i = 0;
  for (std::unique_ptr<Tile>& tile : tiles){
    int x = (i % (tileWidth * 2)) / 2;
    int y = i % 2;
    int z = i / (tileWidth * 2);

    glm::vec3 pos = glm::vec3(0.0f);
    pos.y = startY + model::tileScale.y * y;

    int xMove = std::cos(glm::radians(90.0f * z));
    int zMove = -std::sin(glm::radians(90.0f * z));
    pos.x = r * xMove; 
    pos.z = r * zMove;

    pos.x += offset * zMove; 
    pos.z += -offset * xMove;

    pos.x += x * model::tileScale.x * zMove; 
    pos.z -= x * model::tileScale.x * xMove;

    tile->position = pos;

    glm::quat orientation = glm::quat(glm::vec3(glm::radians(180.0f), glm::radians(90.0f * (z + 1)), 0.0f));
    tile->orientation = orientation;

    i++;
  }

}

void dealHands(int roll, int oya, std::vector<std::unique_ptr<Tile>>& tiles){
  int tileWidth;
  int walls;
  if (tiles.size() == 108){
    tileWidth = 18;
    walls = 3;
  }else {
    tileWidth = 17;
    walls = 4;
  }

  int wall = (roll + oya) % walls;
  wall -= 1;
  if (wall < 0) {
    wall += walls;
  }

  int index = (wall + 1) * tileWidth * 2 - 1;
  index -= roll * 2;


  float r = model::handRadiusDistance;
  float startY = model::tileScale.z / 2.0f;
  float offset = -model::tileScale.x * (13.0f / 2.0f - 0.5f);

  for (int i = 0; i < walls * 4 * 3; i++){
    std::unique_ptr<Tile>& tile = tiles[index];

    int y = i / (walls * 4);
    int x = i % 4 + y * 4;
    int z = ((i / 4) + oya) % walls;

    glm::vec3 pos = glm::vec3(0.0f);
    pos.y = startY;

    int xMove = std::cos(glm::radians(90.0f * z));
    int zMove = -std::sin(glm::radians(90.0f * z));
    pos.x = r * xMove; 
    pos.z = r * zMove;

    pos.x += offset * zMove; 
    pos.z += -offset * xMove;

    pos.x += x * model::tileScale.x * zMove; 
    pos.z -= x * model::tileScale.x * xMove;

    tile->position = pos;

    glm::quat orientation = glm::quat(glm::vec3(glm::radians(90.0f), glm::radians(90.0f * (z + 1)), 0.0f));
    tile->orientation = orientation;

    index -= 1;
    if (index < 0){
      index += tiles.size();
    }
  }

  for (int i = 0; i < 4; i++){
    std::unique_ptr<Tile>& tile = tiles[index];

    int y = 3;
    int x = y * 4;
    int z = (i + oya) % walls;

    glm::vec3 pos = glm::vec3(0.0f);
    pos.y = startY;

    int xMove = std::cos(glm::radians(90.0f * z));
    int zMove = -std::sin(glm::radians(90.0f * z));
    pos.x = r * xMove; 
    pos.z = r * zMove;

    pos.x += offset * zMove; 
    pos.z += -offset * xMove;

    pos.x += x * model::tileScale.x * zMove; 
    pos.z -= x * model::tileScale.x * xMove;

    tile->position = pos;

    glm::quat orientation = glm::quat(glm::vec3(glm::radians(90.0f), glm::radians(90.0f * (z + 1)), 0.0f));
    tile->orientation = orientation;


    index -= 1;
    if (index < 0){
      index += tiles.size();
    }
  }

  std::unique_ptr<Tile>& tile = tiles[index];
  int x = 3 * 4 + 1;
  int z = oya;

  glm::vec3 pos = glm::vec3(0.0f);
  pos.y = startY;

  int xMove = std::cos(glm::radians(90.0f * z));
  int zMove = -std::sin(glm::radians(90.0f * z));
  pos.x = r * xMove; 
  pos.z = r * zMove;

  pos.x += offset * zMove; 
  pos.z += -offset * xMove;

  pos.x += x * model::tileScale.x * zMove + zMove * (model::tileScale.x / 2.0f); 
  pos.z -= x * model::tileScale.x * xMove + xMove * (model::tileScale.x / 2.0f);

  tile->position = pos;

  glm::quat orientation = glm::quat(glm::vec3(glm::radians(90.0f), glm::radians(90.0f * (z + 1)), 0.0f));
  tile->orientation = orientation;
}


void makeDeadWall(int roll, int oya, std::vector<std::unique_ptr<Tile>>& tiles){
  int tileWidth;
  int doraRow;
  int walls;
  if (tiles.size() == 108){
    tileWidth = 18;
    doraRow = 5;
    walls = 3;
  }else {
    tileWidth = 17;
    doraRow = 3;
    walls = 4;
  }

  int wall = (roll + oya) % walls;
  wall -= 1;
  if (wall < 0) {
    wall += walls;
  }

  int index = (wall + 1) * tileWidth * 2;
  index -= roll * 2;

  float r = model::wallRadiusDistance;
  float startY = model::tileScale.y / 2.0f;
  float offset = -model::tileScale.x * (tileWidth / 2.0f - 0.5f);

  int endIndex = (index + 13) % tiles.size();
  if (endIndex / (tileWidth * 2) != index / (tileWidth * 2)){
    for (int i = 0; i < 14; ++i){
      int index = endIndex - i;
      if (index < 0){
        index += tiles.size();
      }

      std::unique_ptr<Tile>& tile = tiles[index];

      int x = i / 2;
      int y = (i + 1) % 2;
      int z = wall;

      glm::vec3 pos = glm::vec3(0.0f);
      pos.y = startY + model::tileScale.y * y;

      int xMove = std::cos(glm::radians(90.0f * z));
      int zMove = -std::sin(glm::radians(90.0f * z));
      pos.x = r * xMove; 
      pos.z = r * zMove;

      pos.x -= offset * zMove; 
      pos.z += offset * xMove;

      pos.x -= x * model::tileScale.x * zMove; 
      pos.z += x * model::tileScale.x * xMove;

      tile->position = pos;

      glm::quat orientation = glm::quat(glm::vec3(glm::radians(180.0f), glm::radians(90.0f * (z + 1)), 0.0f));
      tile->orientation = orientation;
    }
  }else if (roll != 7){
    for (int i = 0; i < 14; ++i){
      std::unique_ptr<Tile>& tile = tiles[index + i];

      int z = wall;

      int xMove = std::cos(glm::radians(90.0f * z));
      int zMove = -std::sin(glm::radians(90.0f * z));

      tile->position.x -= model::tileScale.x * zMove; 
      tile->position.z += model::tileScale.x * xMove;
    }

  }

  std::unique_ptr<Tile>& doraInd = tiles[(index + doraRow * 2 - 1) % tiles.size()];
  glm::quat orientation = glm::quat(glm::vec3(0.0f, glm::radians(90.0f * (wall + 1)), 0.0f));
  doraInd->orientation = orientation;
}




}
