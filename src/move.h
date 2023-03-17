#pragma once
#include <cinttypes>

#include "Board.h"
#include "bitmap.h"
#include "x86utils.h"

struct move_t {
 public:
  const Board& prev_board;
  const bitmap_t origin;
  const bitmap_t target;
  move_t(const Board& prev_board, const bitmap_t origin, const bitmap_t target)
      : prev_board(prev_board), origin(origin), target(target) {}
};

/*
struct move_t {
 private:
  const uint32_t bits;
  const static uint32_t OriginMask = 0xF3F;
  const static uint32_t OriginShift = 0;
  const static uint32_t TargetMask = 0x1F0000;
  const static uint32_t TargetShift = 16;
  const static uint32_t FigureMask = 0x7000000;
  const static uint32_t FigureShift = 24;
  const static uint32_t FlagMask = 0xF8000000;

 public:
  const static uint32_t FLAG_CASTLE_RIGHT = ((uint32_t)1) << 27;
  const static uint32_t FLAG_CASTLE_LEFT = ((uint32_t)1) << 28;
  const static uint32_t FLAG_EN_PASSANT = ((uint32_t)1) << 29;
  const static uint32_t FLAG_CHECK = ((uint32_t)1) << 30;
  const static uint32_t FLAG_NONE = 0;
  move_t(bitmap_t origin, bitmap_t target, figure_type figure, uint32_t flags)
      : bits(SQUARE_OF(origin) | (SQUARE_OF(target) << TargetShift) |
             (figure << FigureShift) | (flags & FlagMask)) {}

  [[nodiscard]] uint32_t origin_square() const {
    return (bits & OriginMask) >> OriginShift;
  }

  [[nodiscard]] bitmap_t origin_bitmap() const {
    return ((bitmap_t)1) << (origin_square());
  }

  [[nodiscard]] uint32_t target_square() const {
    return (bits & TargetMask) >> TargetShift;
  }

  [[nodiscard]] bitmap_t target_bitmap() const {
    return ((bitmap_t)1) << (target_square());
  }

  [[nodiscard]] figure_type figure() const {
    return (figure_type) ((bits & FigureMask) >> FigureShift);
  }

  [[nodiscard]] uint32_t flags() const {
    return (bits & FlagMask);
  }
};
 */
