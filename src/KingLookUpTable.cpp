
#include "KingLookUpTable.h"
KingLookUpTable::KingLookUpTable() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      bitmap_t lookUp = 0x0;
      for (int rx = std::max(row - 1, 0); rx < std::min(row + 1, 7); rx++) {
        for (int cx = std::max(column - 1, 0); cx < std::min(column + 1, 7);
             cx++) {
          lookUp |= ((bitmap_t)1ULL) << (rx * 8 + cx);
        }
      }
      psudo_legal_moves[row * 8 + column] = lookUp;
    }
  }
}

KingLookUpTable& KingLookUpTable::get() {
  static KingLookUpTable instance;  // some cpp weirdness
  return instance;
}
