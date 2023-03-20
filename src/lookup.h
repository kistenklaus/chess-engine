//
// Created by karl on 19/03/23.
//
#pragma once

#include <cassert>

#include "bitmap.h"
#include "tile_index.h"

// lookup table for east horizontal sliding moves
class EastSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65]{};
  EastSlidingLookUpTable();

 public:
  EastSlidingLookUpTable(EastSlidingLookUpTable const&) = delete;
  EastSlidingLookUpTable& operator=(EastSlidingLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const EastSlidingLookUpTable& get();
};

class KingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65]{};
  KingLookUpTable();

 public:
  KingLookUpTable(KingLookUpTable const&) = delete;
  KingLookUpTable& operator=(KingLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    assert(tile_index >= 0);
    assert(tile_index < 64);
    return psudo_legal_moves[tile_index];
  }

  static const KingLookUpTable& get();
};

class KnightLookUpTable {
 private:
  bitmap_t psudo_legal_moves[64]{};
  KnightLookUpTable();

 public:
  KnightLookUpTable(KnightLookUpTable const&) = delete;
  KnightLookUpTable& operator=(KnightLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    assert(tile_index >= 0);
    assert(tile_index < 64);
    return psudo_legal_moves[tile_index];
  }

  static const KnightLookUpTable& get();
};

// lookup table for north horizontal sliding moves
class NorthEastSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65]{};
  NorthEastSlidingLookUpTable();

 public:
  NorthEastSlidingLookUpTable(NorthEastSlidingLookUpTable const&) = delete;
  NorthEastSlidingLookUpTable& operator=(NorthEastSlidingLookUpTable const&) =
      delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const NorthEastSlidingLookUpTable& get();
};

// lookup table for north horizontal sliding moves
class NorthSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65]{};
  NorthSlidingLookUpTable();

 public:
  NorthSlidingLookUpTable(NorthSlidingLookUpTable const&) = delete;
  NorthSlidingLookUpTable& operator=(NorthSlidingLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const NorthSlidingLookUpTable& get();
};

// lookup table for north horizontal sliding moves
class NorthWestSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65]{};
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

// lookup table for north horizontal sliding moves
class SouthEastSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65]{};
  SouthEastSlidingLookUpTable();

 public:
  SouthEastSlidingLookUpTable(SouthEastSlidingLookUpTable const&) = delete;
  SouthEastSlidingLookUpTable& operator=(SouthEastSlidingLookUpTable const&) =
      delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const SouthEastSlidingLookUpTable& get();
};

// lookup table for south horizontal sliding moves
class SouthSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65]{};
  SouthSlidingLookUpTable();

 public:
  SouthSlidingLookUpTable(SouthSlidingLookUpTable const&) = delete;
  SouthSlidingLookUpTable& operator=(SouthSlidingLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const SouthSlidingLookUpTable& get();
};

// lookup table for south horizontal sliding moves
class SouthWestSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65]{};
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

// lookup table for west horizontal sliding moves
class WestSlidingLookUpTable {
 private:
  bitmap_t psudo_legal_moves[65]{};
  WestSlidingLookUpTable();

 public:
  WestSlidingLookUpTable(WestSlidingLookUpTable const&) = delete;
  WestSlidingLookUpTable& operator=(WestSlidingLookUpTable const&) = delete;

  inline bitmap_t operator[](const tile_index_t tile_index) const {
    return psudo_legal_moves[tile_index];
  }

  static const WestSlidingLookUpTable& get();
};
