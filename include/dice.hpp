#ifndef DICE_H
#define DICE_H

#include <memory>

class Dice {
public:
  Dice();

private:
};


namespace dice {
int getDiceRoll();

void setup();

extern std::unique_ptr<Dice> die[2];
}

#endif
