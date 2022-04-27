#include <iostream>

#include "Board.h"
#include "BoardState.h"
#include "fen.h"
#include "lookup.h"
#include "move_generation.h"
#include "x86utils.h"

static bitmap_t tmp;

int main() {
  // const Board board =
  //     fen::parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  const Board board = fen::parse("8/4k3/8/8/8/1p6/PrP1K3/8 w - - 0 1");

  std::cout << board << std::endl;

  generate_moves<BoardState(BLACK, false, true, true, true, true)>(
      board, [](const move_t move) {
        uint64_t from = SQUARE_OF(move.origin);
        uint64_t to = SQUARE_OF(move.target);
        tmp |= move.target;
        std::cout << "move{from:" << from << ",to:" << to << "}" << std::endl;
      });
  std::cout << bitmap_to_bitboard_string(tmp) << std::endl;
}
