#include "H4SlidingLookUpTable.hpp"

H4SlidingLookUpTable::H4SlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = row + -1; i >= 0; i--) {
        map |= ((bitmap_t)1) << (i * 8 + column);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
  psudo_legal_moves[64] = 0;
}

const H4SlidingLookUpTable& H4SlidingLookUpTable::get() {
  static H4SlidingLookUpTable instance;
  return instance;
}
