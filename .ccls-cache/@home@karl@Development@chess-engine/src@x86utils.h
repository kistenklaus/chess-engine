#pragma once

#include <immintrin.h>

#define SQUARE_OF(X) _tzcnt_u64(X)

#define iterate_bits(i, bits)                                      \
  for (bitmap_t __blsr = _blsr_u64(bits), i = __blsr ^ bits; bits; \
       bits = __blsr, __blsr = _blsr_u64(bits), i = bits ^ __blsr)
