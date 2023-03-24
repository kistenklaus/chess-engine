#pragma once

#include <immintrin.h>

#define SQUARE_OF(X) _tzcnt_u64(X)

#define iterate_bits(i, bits)                                      \
  for (bitmap_t __blsr = _blsr_u64(bits), i = __blsr ^ bits; bits; \
       bits = __blsr, __blsr = _blsr_u64(bits), i = bits ^ __blsr)

#define COUNT_TRAILING_ZEROS(x) (_tzcnt_u64(x))
#define REST_LOWEST_BIT(x) (_blsr_u64(x))
#define POP_LOWEST_BIT(x) (_blsi_u64(x))
#define WHILE_RESET_LSB(X) for(;X; X = _blsr_u64(X))


#ifdef _MSC_VER
#define Bitcount(X) __popcnt64(X)
#define compiletime __forceinline static constexpr
#define force_inline __forceinline
#elif defined(__clang__)
#define Bitcount(X) static_cast<uint64_t>(__builtin_popcountll(X))
#define compiletime __attribute__((always_inline)) static constexpr
#define force_inline __attribute__((always_inline)) inline
#elif defined(__GNUC__)
#define compiletime __attribute__((always_inline)) static constexpr
#define force_inline __attribute__((always_inline))
#define BitCount(X) static_cast<uint64_t>(__builtin_popcountll(X))
#else
#define compiletime static constexpr
#define force_inline inline
#endif

inline static uint64_t popBit(uint64_t& val){
  uint64_t lsb = _blsi_u64(val);
  val ^= lsb;
  return lsb;
}