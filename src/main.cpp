
#include <chrono>
#include <iostream>

#include "MagicLookup.h"
#include "fen.h"
#include "perfT.h"

int main() {
  perfT::run(fen::DEFAULT, 0);
  perfT::run(fen::DEFAULT, 1);
  perfT::run(fen::DEFAULT, 2);
  perfT::run(fen::DEFAULT, 3);
  perfT::run(fen::DEFAULT, 4);
  perfT::run(fen::DEFAULT, 5);
}
