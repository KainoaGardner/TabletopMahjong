#include "../include/dice.hpp"
#include <memory>
#include <random>

Dice::Dice(){

}

namespace dice {

int getDiceRoll(){
  std::random_device rd;
  std::mt19937 gen(rd());

  int min = 1;
  int max = 6;
  std::uniform_int_distribution<> distrib(min, max);

  int face = distrib(gen);
  return face;
}

std::unique_ptr<Dice> game[2] = {std::make_unique<Dice>(), std::make_unique<Dice>()};

void setup(){
}


}
