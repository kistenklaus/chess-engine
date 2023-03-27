#pragma once
#include <cinttypes>
#include <iostream>

#include "bitmap.h"
#include "x86utils.h"
#include "figure.h"

enum move_flag {
  MOVE_FLAG_SILENT,
  MOVE_FLAG_SHORT_CASTLE,
  MOVE_FLAG_LONG_CASTLE,
  MOVE_FLAG_DOUBLE_PAWN_PUSH,
  MOVE_FLAG_LEFT_ROOK,
  MOVE_FLAG_RIGHT_ROOK,
  MOVE_FLAG_CAPTURE,
  MOVE_FLAG_EN_PASSANT,
  MOVE_FLAG_PROMOTE
};

template<figure figure, move_flag flag>
struct compiletime_move{
  bitmap_t m_origin;
  bitmap_t m_target;
};

struct runtime_move {
  bitmap_t m_origin;
  bitmap_t m_target;
  figure m_figure;
  move_flag m_flag;
};
