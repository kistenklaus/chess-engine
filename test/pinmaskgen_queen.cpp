//
// Created by karl on 18/03/23.
//

#include <gtest/gtest.h>

#include "Board.h"
#include "bitmap.h"
#include "fen.h"
#include "pinmask_generation.h"
#include "test_utils.h"

TEST(pinmaskgen_queen, white_queen_one_blocker_up_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("7k/1K6/8/3P4/8/8/6b1/8 w - - 0 1");
  const pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.d, 4432676798464, board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
}

TEST(pinmaskgen_queen, black_queen_one_blocker_up_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("7k/1K6/8/3P4/8/8/6b1/8 w - - 0 1").invertBoard();
  const pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.d, 4432676798464, board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
}

TEST(pinmaskgen_queen, white_queen_one_blocker_up_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/6K1/8/4P3/8/8/1b6/7k w - - 0 1");
  const pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.d, 35253226045952, board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
}

TEST(pinmaskgen_queen, black_queen_one_blocker_up_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/8/4P3/8/8/1b6/7k w - - 0 1").invertBoard();
  const pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.d, 35253226045952, board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
}

TEST(pinmaskgen_queen, white_queen_one_blocker_down_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1b6/8/8/4P3/8/6K1/k7 w - - 0 1");
  const pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.d, 567382630203392, board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
}

TEST(pinmaskgen_queen, black_queen_one_blocker_down_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1b6/8/8/4P3/8/6K1/k7 w - - 0 1").invertBoard();
  const pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.d, 567382630203392, board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
}

TEST(pinmaskgen_queen, white_queen_one_blocker_down_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("k7/6b1/8/8/3P4/8/1K6/8 w - - 0 1");
  const pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.d, 0x40201008040000, board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
}

TEST(pinmaskgen_queen, black_queen_one_blocker_down_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("k7/6b1/8/8/3P4/8/1K6/8 w - - 0 1").invertBoard();
  const pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.d, 0x40201008040000, board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
}

//HV

TEST(pinmaskgen_queen, white_queen_one_blocker_down) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1r6/8/8/1B6/8/1K6/7k w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 565157600296960, board);
  EXPECT_BITBOARD(pinmask.d, 0, board);
}

TEST(pinmaskgen_queen, white_queen_one_blocker_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1r2B1K1/8/8/8/8/8/k7 w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0x3E000000000000, board);
  EXPECT_BITBOARD(pinmask.d, 0, board);
}

TEST(pinmaskgen_queen, white_queen_one_blocker_up) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/6K1/8/6B1/8/8/6r1/k7 w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0x404040404000, board);
  EXPECT_BITBOARD(pinmask.d, 0, board);
}

TEST(pinmaskgen_queen, white_queen_one_blocker_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("7k/8/8/8/8/8/1K1B2r1/8 w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0x7C00, board);
  EXPECT_BITBOARD(pinmask.d, 0, board);
}

TEST(pinmaskgen_queen, white_queen_two_blocker_down) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1r6/8/1B6/1B6/8/1K6/7k w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
  EXPECT_BITBOARD(pinmask.d, 0, board);
}

TEST(pinmaskgen_queen, white_queen_two_blocker_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("7k/8/8/8/8/8/1K1BB1r1/8 w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
  EXPECT_BITBOARD(pinmask.d, 0, board);
}

TEST(pinmaskgen_queen, white_queen_two_blocker_up) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/6K1/8/6B1/6P1/8/6r1/k7 w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
  EXPECT_BITBOARD(pinmask.d, 0, board);
}

TEST(pinmaskgen_queen, white_queen_two_blocker_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1r1PB1K1/8/8/8/8/8/k7 w - - 0 1");
  pinmask_t pinmask = generate_pinmask<state>(board);
  EXPECT_BITBOARD(pinmask.hv, 0, board);
  EXPECT_BITBOARD(pinmask.d, 0, board);
}

