//
// Created by karl on 19/03/23.
//

#include "lookup.h"

EastSlidingLookUpTable::EastSlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = column + 1; i < 8; i++) {
        map |= ((bitmap_t)1) << (row * 8 + i);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
  psudo_legal_moves[64] = 0;
}

const EastSlidingLookUpTable& EastSlidingLookUpTable::get() {
  static EastSlidingLookUpTable instance;
  return instance;
}

KingLookUpTable::KingLookUpTable() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      bitmap_t lookUp = 0x0;
      for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
          int rx = std::min(std::max(row + y, 0), 7);
          int ry = std::min(std::max(column + x, 0), 7);
          lookUp |= (((bitmap_t)1ULL) << (rx * 8 + ry));
        }
      }
      const tile_index_t index = row * 8 + column;
      lookUp &= ~(((bitmap_t)0x1ULL) << index);
      psudo_legal_moves[index] = lookUp;
    }
  }
}

const KingLookUpTable& KingLookUpTable::get() {
  static KingLookUpTable instance;  // some cpp weirdness
  return instance;
}

KnightLookUpTable::KnightLookUpTable() {
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      bitmap_t lookUp = 0;
      const tile_index_t tile_index = row * 8 + column;
      if (row >= 2 && column >= 1) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index - 17);
      }
      if (row >= 1 && column >= 2) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index - 10);
      }
      if (row >= 2 && column < 7) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index - 15);
      }
      if (row >= 1 && column < 6) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index - 6);
      }
      if (row < 7 && column >= 2) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index + 6);
      }
      if (row < 6 && column >= 1) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index + 15);
      }
      if (row < 6 && column < 7) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index + 17);
      }
      if (row < 7 && column < 6) {
        lookUp |= ((bitmap_t)1ULL) << (tile_index + 10);
      }
      this->psudo_legal_moves[tile_index] = lookUp;
    }
  }
}

const KnightLookUpTable& KnightLookUpTable::get() {
  static KnightLookUpTable instance;  // some cpp weirdness
  return instance;
}

NorthEastSlidingLookUpTable::NorthEastSlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = row + 1, j = column + 1; i < 8 && j < 8; i++, j++) {
        map |= ((bitmap_t)1) << (i * 8 + j);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
  psudo_legal_moves[64] = 0;
}

const NorthEastSlidingLookUpTable& NorthEastSlidingLookUpTable::get() {
  static NorthEastSlidingLookUpTable instance;
  return instance;
}

NorthSlidingLookUpTable::NorthSlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = row + 1; i < 8; i++) {
        map |= ((bitmap_t)1) << (i * 8 + column);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
  psudo_legal_moves[64] = 0;
}

const NorthSlidingLookUpTable& NorthSlidingLookUpTable::get() {
  static NorthSlidingLookUpTable instance;
  return instance;
}

NorthWestSlidingLookUpTable::NorthWestSlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = row + 1, j = column - 1; i < 8 && j >= 0; i++, j--) {
        map |= ((bitmap_t)1) << (i * 8 + j);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
  psudo_legal_moves[64] = 0;
}

const NorthWestSlidingLookUpTable& NorthWestSlidingLookUpTable::get() {
  static NorthWestSlidingLookUpTable instance;
  return instance;
}

SouthEastSlidingLookUpTable::SouthEastSlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = row + -1, j = column + 1; i >= 0 && j < 8; i--, j++) {
        map |= ((bitmap_t)1) << (i * 8 + j);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
  psudo_legal_moves[64] = 0;
}

const SouthEastSlidingLookUpTable& SouthEastSlidingLookUpTable::get() {
  static SouthEastSlidingLookUpTable instance;
  return instance;
}

SouthSlidingLookUpTable::SouthSlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = row + -1; i >= 0; i--) {
        map |= ((bitmap_t)1) << (i * 8 + column);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
  psudo_legal_moves[64] = 0;
}

const SouthSlidingLookUpTable& SouthSlidingLookUpTable::get() {
  static SouthSlidingLookUpTable instance;
  return instance;
}

SouthWestSlidingLookUpTable::SouthWestSlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = row + -1, j = column - 1; i >= 0 && j >= 0; i--, j--) {
        map |= ((bitmap_t)1) << (i * 8 + j);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
  psudo_legal_moves[64] = 0;
}

const SouthWestSlidingLookUpTable& SouthWestSlidingLookUpTable::get() {
  static SouthWestSlidingLookUpTable instance;
  return instance;
}

WestSlidingLookUpTable::WestSlidingLookUpTable() {
  //
  for (int row = 0; row < 8; row++) {
    for (int column = 0; column < 8; column++) {
      //
      bitmap_t map = 0;
      for (int i = column - 1; i >= 0; i--) {
        map |= ((bitmap_t)1) << (row * 8 + i);
      }
      psudo_legal_moves[row * 8 + column] = map;
    }
  }
  psudo_legal_moves[64] = 0;
}

const WestSlidingLookUpTable& WestSlidingLookUpTable::get() {
  static WestSlidingLookUpTable instance;
  return instance;
}
