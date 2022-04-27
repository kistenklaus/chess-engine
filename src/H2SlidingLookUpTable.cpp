#include "H2SlidingLookUpTable.hpp"

H2SlidingLookUpTable::H2SlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = 0; i < row; i++) {
        row |= ((bitmap_t)1) << (row * 8 + column);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
}

const H2SlidingLookUpTable& H2SlidingLookUpTable::get() {
  static H2SlidingLookUpTable instance;
  return instance;
}
