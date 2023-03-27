
#include <chrono>
#include <iostream>

#include "perfT.h"
#include "ChessPosition.h"
#include "cli.h"

int main(int argc, char* argv[]) {
  cli::run();

  ChessPosition position = ChessPosition::StartPosition();

  perfT::run(position, 1);
  perfT::run(position, 2);
  perfT::run(position, 3);
  perfT::run(position, 4);
}
