#pragma once

#include <cassert>
#include <iostream>

#include "bitmap.h"
#include "tile_index.h"

class DSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[64];
  DSlidingLookUpTable();

 public:
  DSlidingLookUpTable(DSlidingLookUpTable const&) = delete;
  DSlidingLookUpTable& operator=(DSlidingLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    assert(tile_index >= 0);
    assert(tile_index < 64);
    return psudo_legal_moves[tile_index];
  }

  static const DSlidingLookUpTable& get();
};
