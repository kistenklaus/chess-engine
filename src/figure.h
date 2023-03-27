#pragma once
#include <cinttypes>
#include <iostream>
#include <memory>

#include "color.h"

enum figure : uint8_t {
  PAWN = 1,
  BISHOP = 2,
  KNIGHT = 3,
  ROOK = 4,
  QUEEN = 5,
  KING = 6
};