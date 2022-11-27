#include "bitmap.h"

std::string bitmap_to_bitboard_string(bitmap_t bitmap) {
  std::stringstream stream;
  for (int row = 7; row >= 0; row--) {
    for (int column = 0; column < 8; column++) {
      uint8_t tile_index = row * 8 + column;

      std::string occ = ((bitmap & (0x1ULL << tile_index)) != 0) ? "[]" : "  ";
      stream << "\x1B[30m";
      if ((column + row * 9) % 2 == 1) {
        stream << "\x1B[43m" << occ << "\033[0m";
      } else {
        stream << "\x1B[44m" << occ << "\033[0m";
      }
      stream << "\033[0m";
    }
    stream << std::endl;
  }

  return stream.str();
}
