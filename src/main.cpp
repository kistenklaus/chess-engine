#include <immintrin.h>

#include <iostream>

#include "Board.h"
#include "fen.h"
#include "lookup.h"

#define SquareOf(X) _tzcnt_u64(X)
//#define Bitloop(X) for (; X; X = _blsr_u64(X))

#define Bitloop(BITSET)

#define iterate_bits(i, bits)                                      \
  for (uint64_t __blsr = _blsr_u64(bits), i = __blsr ^ bits; bits; \
       bits = __blsr, __blsr = _blsr_u64(bits), i = bits ^ __blsr)

int main() {
  /*
  Board board = board_from_fen("not yet implemented");
  std::cout << board << std::endl;

  for (int i = 0; i < 64; i++) {
    bitmap_t lookUp = DSlidingLookUpTable::get()[i];
    std::cout << i << std::endl;
    std::cout << bitmap_to_bitboard_string(lookUp);
    std::cout << std::endl;
  }
  */

  unsigned long long bitset = 0xF;
  uint64_t i;
  iterate_bits(i, bitset) { std::cout << i << std::endl; }
}
