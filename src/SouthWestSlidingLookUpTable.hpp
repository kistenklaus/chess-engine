#pragma once

#include <cassert>

#include "bitmap.h"
#include "tile_index.h"

// lookup table for south horizontal sliding moves
class SouthWestSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65];
  SouthWestSlidingLookUpTable();

 public:
  SouthWestSlidingLookUpTable(SouthWestSlidingLookUpTable const&) = delete;
  SouthWestSlidingLookUpTable& operator=(SouthWestSlidingLookUpTable const&) =
      delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const SouthWestSlidingLookUpTable& get();
};
