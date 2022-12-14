#include "SouthWestSlidingLookUpTable.hpp"

SouthWestSlidingLookUpTable::SouthWestSlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = row + -1, j = column - 1; i >= 0 && j >= 0; i--, j--) {
        map |= ((bitmap_t)1) << (i * 8 + j);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
  psudo_legal_moves[64] = 0;
}

const SouthWestSlidingLookUpTable& SouthWestSlidingLookUpTable::get() {
  static SouthWestSlidingLookUpTable instance;
  return instance;
}
