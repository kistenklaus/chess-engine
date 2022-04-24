#pragma once

#include "Board.h"
#include "bitmap.h"

struct move_t {
  const Board& prev_board;
  const bitmap_t origin;
  const bitmap_t target;
  move_t(Board& prev_board, bitmap_t origin, bitmap_t target)
      : prev_board(prev_board), origin(origin), target(target) {}
};
