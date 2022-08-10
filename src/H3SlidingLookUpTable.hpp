#pragma once

#include <cassert>

#include "bitmap.h"
#include "tile_index.h"

// lookup table for north horizontal sliding moves
class H3SlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65];
  H3SlidingLookUpTable();

 public:
  H3SlidingLookUpTable(H3SlidingLookUpTable const&) = delete;
  H3SlidingLookUpTable& operator=(H3SlidingLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const H3SlidingLookUpTable& get();
};
