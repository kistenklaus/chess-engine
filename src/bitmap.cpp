//
// Created by karl on 28/03/23.
//
#include "bitmap.h"

std::string bitmap::toString(bitmap_t map) {
  std::stringstream ss;
  for (int rank = 7; rank >= 0; rank--) {
    for (int file = 0; file < 8; file++) {
      bitmap_t mask = 1ull << (rank * 8 + file);
      if (mask & map) {
        ss << "";
      } else {
        ss << "";
      }
      ss << ' ';
    }
    ss << '\n';
  }
  return ss.str();
}
