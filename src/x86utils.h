#pragma once

#include <immintrin.h>

#define SQUARE_OF(X) _tzcnt_u64(X)

#define iterate_bits(i, bits)                                      \
  for (bitmap_t __blsr = _blsr_u64(bits), i = __blsr ^ bits; bits; \
       bits = __blsr, __blsr = _blsr_u64(bits), i = bits ^ __blsr)

#define COUNT_TRAILING_ZEROS(x) (_tzcnt_u64(x))
#define REST_LOWEST_BIT(x) (_blsr_u64(x))
#define POP_LOWEST_BIT(x) (_blsi_u64(x))


