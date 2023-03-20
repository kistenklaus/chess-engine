#pragma once
#include <cinttypes>

#include "Board.h"
#include "bitmap.h"
#include "tile_index.h"
#include "x86utils.h"
#include <iostream>

typedef uint32_t move_flag;

const move_flag MOVE_FLAG_NONE = 0;
const move_flag MOVE_FLAG_RCASTLE = 1;
const move_flag MOVE_FLAG_LCASTLE = 2;
const move_flag MOVE_FLAG_EN_PASSANT = 4;
const move_flag MOVE_FLAG_DOUBLE_PAWN_PUSH = 8;
const move_flag MOVE_FLAG_CAPTURE = 16;
const move_flag MOVE_FLAG_CHECK = 32;


struct runtime_move {
  bitmap_t m_origin;
  bitmap_t m_target;
  figure_type m_figure;
  move_flag m_flags;
  runtime_move(bitmap_t origin, bitmap_t target, figure_type figure,
               move_flag flag)
      : m_origin(origin), m_target(target), m_figure(figure), m_flags(flag) {}
};

std::ostream& operator<<(std::ostream& cout, const runtime_move& move);
