
#include <chrono>
#include <iostream>

#include "MagicLookup.h"
#include "fen.h"
#include "perfT.h"

int main(int argc, char* argv[]) {
  std::string fen = "rnbqkbnr/pppppppp/8/8/8/7P/PPPPPPP1/RNBQKBNR b KQkq - 0 1";
  fen = fen::DEFAULT;
  fen = "rnbqkbnr/pppppppp/8/8/7P/8/PPPPPPP1/RNBQKBNR b KQkq h3 0 1";
  perfT::run(fen, 0);
  perfT::run(fen, 1);
  perfT::run(fen, 2);
  perfT::run(fen, 3);
  perfT::run(fen, 4);
  perfT::run(fen, 5);
  perfT::run(fen, 6);
}
