#pragma once

#include <cassert>

#include "bitmap.h"
#include "tile_index.h"

// lookup table for south horizontal sliding moves
class H4SlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65];
  H4SlidingLookUpTable();

 public:
  H4SlidingLookUpTable(H4SlidingLookUpTable const&) = delete;
  H4SlidingLookUpTable& operator=(H4SlidingLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const H4SlidingLookUpTable& get();
};
