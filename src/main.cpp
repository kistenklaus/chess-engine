#include <immintrin.h>

#include <iostream>

#include "Board.h"
#include "BoardState.h"
#include "fen.h"
#include "lookup.h"
#include "move_generation.h"
#include "x86utils.h"

static bitmap_t tmp;

int main() {
  /*
  Board board = board_from_fen("not yet implemented");
  std::cout << board << std::endl;

  */

  unsigned long long bitset = 0xF;
  uint64_t i;
  iterate_bits(i, bitset) { std::cout << i << std::endl; }

  const Board& board = board_from_fen("");
  std::cout << board << std::endl;

  generate_moves<BoardState::Default()>(board, [](const move_t& move) {
    uint64_t from = SQUARE_OF(move.origin);
    uint64_t to = SQUARE_OF(move.target);
    tmp |= move.target;
    std::cout << "move{from:" << from << ",to:" << to << "}" << std::endl;
  });

  std::cout << bitmap_to_bitboard_string(tmp) << std::endl;
}
