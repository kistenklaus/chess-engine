#include "HVSlidingLookUpTable.h"

HVSlidingLookUpTable::HVSlidingLookUpTable() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      bitmap_t lookUp = 0x0;
      lookUp |= ((bitmap_t)0xFF) << (8 * row);
      lookUp |= ((bitmap_t)0x101010101010101) << column;
      psudo_legal_moves[row * 8 + column] = lookUp;
    }
  }
}

const HVSlidingLookUpTable& HVSlidingLookUpTable::get() {
  static HVSlidingLookUpTable instance;  // some cpp weirdness
  return instance;
}
