//
// Created by karl on 17/03/23.
//

#include <gtest/gtest.h>

#include "Board.h"
#include "bitmap.h"
#include "fen.h"
#include "pinmask_generation.h"
#include "test_utils.h"

TEST(pinmaskgen_rook, white_rook_one_blocker_down) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1r6/8/8/1B6/8/1K6/7k w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 565157600296960, board);
  EXPECT_BITBOARD(pinmask.d12, 0, board);
}

TEST(pinmaskgen_rook, white_rook_one_blocker_right) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1r2B1K1/8/8/8/8/8/k7 w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0x3E000000000000, board);
  EXPECT_BITBOARD(pinmask.d12, 0, board);
}

TEST(pinmaskgen_rook, white_rook_one_blocker_up) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/6K1/8/6B1/8/8/6r1/k7 w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0x404040404000, board);
  EXPECT_BITBOARD(pinmask.d12, 0, board);
}

TEST(pinmaskgen_rook, white_rook_one_blocker_left) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("7k/8/8/8/8/8/1K1B2r1/8 w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0x7C00, board);
  EXPECT_BITBOARD(pinmask.d12, 0, board);
}

TEST(pinmaskgen_rook, white_rook_two_blocker_down) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1r6/8/1B6/1B6/8/1K6/7k w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
  EXPECT_BITBOARD(pinmask.d12, 0, board);
}

TEST(pinmaskgen_rook, white_rook_two_blocker_left) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("7k/8/8/8/8/8/1K1BB1r1/8 w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
  EXPECT_BITBOARD(pinmask.d12, 0, board);
}

TEST(pinmaskgen_rook, white_rook_two_blocker_up) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/6K1/8/6B1/6P1/8/6r1/k7 w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
  EXPECT_BITBOARD(pinmask.d12, 0, board);
}

TEST(pinmaskgen_rook, white_rook_two_blocker_right) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1r1PB1K1/8/8/8/8/8/k7 w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
  EXPECT_BITBOARD(pinmask.d12, 0, board);
}

