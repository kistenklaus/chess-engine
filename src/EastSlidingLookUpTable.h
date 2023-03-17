#pragma once

#include <cassert>

#include "bitmap.h"
#include "tile_index.h"

// lookup table for east horizontal sliding moves
class EastSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65];
  EastSlidingLookUpTable();

 public:
  EastSlidingLookUpTable(EastSlidingLookUpTable const&) = delete;
  EastSlidingLookUpTable& operator=(EastSlidingLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const EastSlidingLookUpTable& get();
};
