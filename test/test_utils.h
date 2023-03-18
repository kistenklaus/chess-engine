//
// Created by karl on 18/03/23.
//
#pragma once

#include <gtest/gtest.h>

#include "Board.h"
#include "bitmap.h"
#include "checkmask.h"
#include "checkmask_generation.h"
#include "fen.h"

inline void EXPECT_BITBOARD(bitmap_t value, bitmap_t expected) {
  EXPECT_EQ(value, expected);
  if (value != expected) {
    std::cout << "=============================================" << std::endl;
    std::cout << "VALUE : " << value << std::endl;
    std::cout << bitmap_to_bitboard_string(value) << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "EXPECTED : " << expected << std::endl;
    std::cout << bitmap_to_bitboard_string(expected) << std::endl;
    std::cout << "=============================================" << std::endl;
  }
}