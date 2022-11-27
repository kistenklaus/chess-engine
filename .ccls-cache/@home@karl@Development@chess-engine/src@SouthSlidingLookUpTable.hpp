#pragma once

#include <cassert>

#include "bitmap.h"
#include "tile_index.h"

// lookup table for south horizontal sliding moves
class SouthSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65];
  SouthSlidingLookUpTable();

 public:
  SouthSlidingLookUpTable(SouthSlidingLookUpTable const&) = delete;
  SouthSlidingLookUpTable& operator=(SouthSlidingLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const SouthSlidingLookUpTable& get();
};
