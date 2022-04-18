#include <iostream>

#include "Board.h"
#include "KnightLookUpTable.h"
#include "fen.h"

int main() {
  Board board = board_from_fen("not yet implemented");
  std::cout << board << std::endl;

  for (int i = 0; i < 64; i++) {
    bitmap_t lookUp = KnightLookUpTable::get()[i];
    std::cout << i << std::endl;
    std::cout << bitmap_to_bitboard_string(lookUp);
    std::cout << std::endl;
  }
}
