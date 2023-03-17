#pragma once
#include "cinttypes"
#include "color.h"
#include "iostream"
#include "memory"

#define FIGURE_T_TYPE_MASK (0b111)
#define FIGURE_T_COLOR_MASK (1 << 7)

enum figure_type : uint8_t{
  NOT_A_FIGURE = 0,
  PAWN = 1,
  BISHOP = 2,
  KNIGHT = 3,
  ROOK = 4,
  QUEEN = 5,
  KING = 6
};

struct figure_t {
 private:
  uint8_t bits;

 public:
  figure_t() : bits(0) {}
  explicit figure_t(uint8_t bits) : bits(bits) {}
  figure_t(color_t color, figure_type figure_type)
      : bits(figure_type | (color << 7)) {}

  [[nodiscard]] inline figure_type type() const {
    return static_cast<figure_type>(bits & FIGURE_T_TYPE_MASK);
  }

  [[deprecated]] [[nodiscard]] inline color_t color() const {
    return ((bits & FIGURE_T_COLOR_MASK) << 7) != 0;
  }
};

std::ostream &operator<<(std::ostream &cout, const figure_t &figure);
