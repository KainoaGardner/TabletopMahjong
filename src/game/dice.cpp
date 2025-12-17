#include "../include/game/dice.hpp"
#include <random>

int Dice::getDiceRoll(){
  std::random_device rd;
  std::mt19937 gen(rd());

  int min = 1;
  int max = 6;
  std::uniform_int_distribution<> distrib(min, max);

  int face = distrib(gen);
  return face;
}
