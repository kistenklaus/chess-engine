//
// Created by karl on 18/03/23.
//
#pragma once

#include <gtest/gtest.h>

#include <iostream>

#include "Board.h"
#include "bitmap.h"
#include "checkmask.h"
#include "checkmask_generation.h"
#include "fen.h"

#define EXPECT_BITMAP(value, expected)                                         \
  EXPECT_EQ(value, expected);                                                  \
  if (value != expected) {                                                     \
    std::cout << "=============================================" << std::endl; \
    std::cout << "VALUE : " << value << std::endl;                             \
    std::cout << bitmap_to_bitboard_string(value) << std::endl;                \
    std::cout << "=============================================" << std::endl; \
    std::cout << "EXPECTED : " << expected << std::endl;                       \
    std::cout << bitmap_to_bitboard_string(expected) << std::endl;             \
    std::cout << "=============================================" << std::endl; \
  }

#define EXPECT_BITBOARD(value, expected, board)                                \
  EXPECT_EQ(value, expected);                                                  \
  if (value != expected) {                                                     \
    std::cout << "=============================================" << std::endl; \
    std::cout << board << std::endl;                                           \
    std::cout << "=============================================" << std::endl; \
    std::cout << "VALUE : " << value << std::endl;                             \
    std::cout << bitmap_to_bitboard_string(value) << std::endl;                \
    std::cout << "=============================================" << std::endl; \
    std::cout << "EXPECTED : " << expected << std::endl;                       \
    std::cout << bitmap_to_bitboard_string(expected) << std::endl;             \
    std::cout << "=============================================" << std::endl; \
  }
