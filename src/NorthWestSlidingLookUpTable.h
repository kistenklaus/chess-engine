#pragma once

#include <cassert>

#include "bitmap.h"
#include "tile_index.h"

// lookup table for north horizontal sliding moves
class NorthWestSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65];
  NorthWestSlidingLookUpTable();

 public:
  NorthWestSlidingLookUpTable(NorthWestSlidingLookUpTable const&) = delete;
  NorthWestSlidingLookUpTable& operator=(NorthWestSlidingLookUpTable const&) =
      delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const NorthWestSlidingLookUpTable& get();
};
