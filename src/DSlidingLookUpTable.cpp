#include "DSlidingLookUpTable.h"

DSlidingLookUpTable::DSlidingLookUpTable() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      bitmap_t bitmap = 0x0;

      // Slide top left
      for (int rx = row + 1; rx < 7; rx++) {
        for (int cx = column - 1; cx >= 0; cx--) {
          bitmap |= ((bitmap_t)0x1ULL) << (rx * 8 + cx);
        }
      }
      // Slide top right
      for (int rx = row + 1; rx < 7; rx++) {
        for (int cx = column + 1; cx < 7; cx++) {
          bitmap |= ((bitmap_t)0x1ULL) << (rx * 8 + cx);
        }
      }
      // Slide bottom right
      for (int rx = row - 1; rx >= 0; rx--) {
        for (int cx = column + 1; cx < 7; cx++) {
          bitmap |= ((bitmap_t)0x1ULL) << (rx * 8 + cx);
        }
      }
      // Slide bottom left
      for (int rx = row - 1; rx >= 0; rx--) {
        for (int cx = column - 1; cx >= 0; cx--) {
          bitmap |= ((bitmap_t)0x1ULL) << (rx * 8 + cx);
        }
      }

      psudo_legal_moves[row * 8 + column] = bitmap;
    }
  }
}

const DSlidingLookUpTable& DSlidingLookUpTable::get() {
  static DSlidingLookUpTable instance;  // some cpp weirdness
  return instance;
}
