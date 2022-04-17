#pragma once
#include "cinttypes"
#include "color.h"
#include "iostream"
#include "memory"

#define FIGURE_T_TYPE_MASK (~1)
#define FIGURE_T_COLOR_MASK 1

enum figure_type {
  EMPTY = 0,
  PAWN = 2,
  BISHOP = 4,
  KNIGHT = 6,
  ROOK = 8,
  QUEEN = 10,
  KING = 12
};

struct figure_t {
 private:
  uint8_t bits;

 public:
  figure_t();
  figure_t(uint8_t bits);
  figure_t(color_t color, figure_type figure_type);
  inline figure_type type() const {
    return static_cast<figure_type>(bits & FIGURE_T_TYPE_MASK);
  }
  inline color_t color() const { return (bits & FIGURE_T_COLOR_MASK) != 0; }
};

std::ostream &operator<<(std::ostream &cout, const figure_t &figure);
