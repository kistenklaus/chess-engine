#pragma once

#include <cassert>

#include "bitmap.h"
#include "tile_index.h"

class H2SlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[64];
  H2SlidingLookUpTable();

 public:
  H2SlidingLookUpTable(H2SlidingLookUpTable const&) = delete;
  H2SlidingLookUpTable& operator=(H2SlidingLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    assert(tile_index >= 0);
    assert(tile_index < 64);
    return psudo_legal_moves[tile_index];
  }

  static const H2SlidingLookUpTable& get();
};
