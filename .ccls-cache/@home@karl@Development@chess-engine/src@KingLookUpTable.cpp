#include "KingLookUpTable.h"

KingLookUpTable::KingLookUpTable() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      bitmap_t lookUp = 0x0;
      for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
          int rx = std::min(std::max(row + y, 0), 7);
          int ry = std::min(std::max(column + x, 0), 7);
          lookUp |= (((bitmap_t)1ULL) << (rx * 8 + ry));
        }
      }
      const tile_index_t index = row * 8 + column;
      lookUp &= ~(((bitmap_t)0x1ULL) << index);
      psudo_legal_moves[index] = lookUp;
    }
  }
}

const KingLookUpTable& KingLookUpTable::get() {
  static KingLookUpTable instance;  // some cpp weirdness
  return instance;
}
