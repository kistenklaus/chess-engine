#pragma once

#include <cassert>
#include <iostream>

#include "bitmap.h"
#include "tile_index.h"

class KnightLookUpTable {
private:
  bitmap_t psudo_legal_moves[64];
  KnightLookUpTable();

public:
  KnightLookUpTable(KnightLookUpTable const &) = delete;
  KnightLookUpTable &operator=(KnightLookUpTable const &) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    assert(tile_index >= 0);
    assert(tile_index < 64);
    return psudo_legal_moves[tile_index];
  }

  static const KnightLookUpTable &get();
};
