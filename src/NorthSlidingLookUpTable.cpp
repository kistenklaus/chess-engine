#include "NorthSlidingLookUpTable.hpp"

NorthSlidingLookUpTable::NorthSlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = row + 1; i < 8; i++) {
        map |= ((bitmap_t)1) << (i * 8 + column);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
  psudo_legal_moves[64] = 0;
}

const NorthSlidingLookUpTable& NorthSlidingLookUpTable::get() {
  static NorthSlidingLookUpTable instance;
  return instance;
}
