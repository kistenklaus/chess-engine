
#include <chrono>
#include <iostream>

#include "MagicLookup.h"
#include "fen.h"
#include "perfT.h"
#include "ChessPosition.h"

int main(int argc, char* argv[]) {
  ChessPosition position = ChessPosition::StartPosition();

  perfT::run(position, 0);
  perfT::run(position, 1);
  perfT::run(position, 2);
  perfT::run(position, 3);
  perfT::run(position, 4);
  perfT::run(position, 5);
  perfT::run(position, 6);
}
