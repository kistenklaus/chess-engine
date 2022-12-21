#include "KnightLookUpTable.h"
KnightLookUpTable::KnightLookUpTable() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      bitmap_t lookUp = 0;
      const tile_index_t tile_index = row * 8 + column;
      if (row >= 2 && column >= 1) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index - 17);
      }
      if (row >= 1 && column >= 2) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index - 10);
      }
      if (row >= 2 && column < 7) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index - 15);
      }
      if (row >= 1 && column < 6) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index - 6);
      }
      if (row < 7 && column >= 2) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index + 6);
      }
      if (row < 6 && column >= 1) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index + 15);
      }
      if (row < 6 && column < 7) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index + 17);
      }
      if (row < 7 && column < 6) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index + 10);
      }
      this->psudo_legal_moves[tile_index] = lookUp;
    }
  }
}

const KnightLookUpTable& KnightLookUpTable::get() {
  static KnightLookUpTable instance;  // some cpp weirdness
  return instance;
}
