#pragma once

#include <cassert>

#include "bitmap.h"
#include "tile_index.h"

// lookup table for west horizontal sliding moves
class WestSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65];
  WestSlidingLookUpTable();

 public:
  WestSlidingLookUpTable(WestSlidingLookUpTable const&) = delete;
  WestSlidingLookUpTable& operator=(WestSlidingLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const WestSlidingLookUpTable& get();
};
