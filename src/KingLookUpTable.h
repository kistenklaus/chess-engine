#pragma once

#include <cassert>
#include <iostream>

#include "bitmap.h"
#include "tile_index.h"

class KingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[64];
  KingLookUpTable();

 public:
  KingLookUpTable(KingLookUpTable const&) = delete;
  KingLookUpTable& operator=(KingLookUpTable const&) = delete;

  bitmap_t operator[](tile_index_t tile_index) {
    assert(tile_index >= 0);
    assert(tile_index < 64);
    return psudo_legal_moves[tile_index];
  }

  static KingLookUpTable& get();
};
