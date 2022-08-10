#include "H1SlidingLookUpTable.hpp"

H1SlidingLookUpTable::H1SlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = column - 1; i >= 0; i--) {
        map |= ((bitmap_t)1) << (row * 8 + i);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
  psudo_legal_moves[64] = 0;
}

const H1SlidingLookUpTable& H1SlidingLookUpTable::get() {
  static H1SlidingLookUpTable instance;
  return instance;
}
