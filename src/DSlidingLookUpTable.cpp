#include "DSlidingLookUpTable.h"

DSlidingLookUpTable::DSlidingLookUpTable() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      bitmap_t lookUp = 0x0;

      // Slide top left
      for (int rx = row + 1, cx = column - 1; rx < 8 && cx >= 0; rx++, cx--) {
        lookUp |= ((bitmap_t)0x1ULL) << (rx * 8 + cx);
      }
      // Slide top right
      for (int rx = row + 1, cx = column + 1; rx < 8 && cx < 8; rx++, cx++) {
        lookUp |= ((bitmap_t)0x1ULL) << (rx * 8 + cx);
      }

      // Slide bottom right
      for (int rx = row - 1, cx = column + 1; rx >= 0 && cx < 8; rx--, cx++) {
        lookUp |= ((bitmap_t)0x1ULL) << (rx * 8 + cx);
      }
      // Slide bottom left
      for (int rx = row - 1, cx = column - 1; rx >= 0 && cx >= 0; rx--, cx--) {
        lookUp |= ((bitmap_t)0x1ULL) << (rx * 8 + cx);
      }

      psudo_legal_moves[row * 8 + column] = lookUp;
    }
  }
}

const DSlidingLookUpTable& DSlidingLookUpTable::get() {
  static DSlidingLookUpTable instance;  // some cpp weirdness
  return instance;
}
