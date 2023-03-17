//
// Created by karl on 17/03/23.
//

#include <gtest/gtest.h>

#include "Board.h"
#include "MoveTestReceiver.h"
#include "bitmap.h"
#include "fen.h"
#include "pinmask_generation.h"

TEST(pinmaskgen_rook, white_rook_one_blocker_down) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board1 = fen::parse("8/1r6/8/8/1B6/8/1k6/7K w - - 0 1");
  pinmask_t pinmask1 = generate_pinmask<state>(board1);
  EXPECT_EQ(pinmask1.hv, 0x2020202000000);
  EXPECT_EQ(pinmask1.d, 0);
}

TEST(pinmaskgen_rook, black_rook_one_blocker_down) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board1 =
      fen::parse("8/1r6/8/8/1B6/8/1k6/7K w - - 0 1").invertBoard();
  pinmask_t pinmask1 = generate_pinmask<state>(board1);
  EXPECT_EQ(pinmask1.hv, 0x2020202000000);
  EXPECT_EQ(pinmask1.d, 0);
}

TEST(pinmaskgen_rook, white_rook_one_blocker_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board4 = fen::parse("8/1r2B1k1/8/8/8/8/8/K7 w - - 0 1");
  pinmask_t pinmask4 = generate_pinmask<state>(board4);
  EXPECT_EQ(pinmask4.hv, 0x3E000000000000);
  EXPECT_EQ(pinmask4.d, 0);
}

TEST(pinmaskgen_rook, black_rook_one_blocker_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board4 =
      fen::parse("8/1r2B1k1/8/8/8/8/8/K7 w - - 0 1").invertBoard();
  pinmask_t pinmask4 = generate_pinmask<state>(board4);
  EXPECT_EQ(pinmask4.hv, 0x3E000000000000);
  EXPECT_EQ(pinmask4.d, 0);
}

TEST(pinmaskgen_rook, white_rook_one_blocker_up) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board3 = fen::parse("8/6k1/8/6B1/8/8/6r1/K7 w - - 0 1");
  pinmask_t pinmask3 = generate_pinmask<state>(board3);
  EXPECT_EQ(pinmask3.hv, 0x404040404000);
  EXPECT_EQ(pinmask3.d, 0);
}

TEST(pinmaskgen_rook, black_rook_one_blocker_up) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board3 =
      fen::parse("8/6k1/8/6B1/8/8/6r1/K7 w - - 0 1").invertBoard();
  pinmask_t pinmask3 = generate_pinmask<state>(board3);
  EXPECT_EQ(pinmask3.hv, 0x404040404000);
  EXPECT_EQ(pinmask3.d, 0);
}

TEST(pinmaskgen_rook, white_rook_one_blocker_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board2 = fen::parse("7K/8/8/8/8/8/1k1B2r1/8 w - - 0 1");
  pinmask_t pinmask2 = generate_pinmask<state>(board2);
  EXPECT_EQ(pinmask2.hv, 0x7C00);
  EXPECT_EQ(pinmask2.d, 0);
}

TEST(pinmaskgen_rook, black_rook_one_blocker_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board2 =
      fen::parse("7K/8/8/8/8/8/1k1B2r1/8 w - - 0 1").invertBoard();
  pinmask_t pinmask2 = generate_pinmask<state>(board2);
  EXPECT_EQ(pinmask2.hv, 0x7C00);
  EXPECT_EQ(pinmask2.d, 0);
}

TEST(pinmaskgen_rook, white_rook_two_blocker_down) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board1 = fen::parse("8/1r6/8/1B6/1B6/8/1k6/7K w - - 0 1");
  pinmask_t pinmask1 = generate_pinmask<state>(board1);
  EXPECT_EQ(pinmask1.hv, 0);
  EXPECT_EQ(pinmask1.d, 0);
}

TEST(pinmaskgen_rook, black_rook_two_blocker_down) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board1 = fen::parse("8/1r6/8/1B6/1B6/8/1k6/7K w - - 0 1")
                           .invertBoard();
  pinmask_t pinmask1 = generate_pinmask<state>(board1);
  EXPECT_EQ(pinmask1.hv, 0);
  EXPECT_EQ(pinmask1.d, 0);
}

TEST(pinmaskgen_rook, white_rook_two_blocker_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board2 = fen::parse("7K/8/8/8/8/8/1k1BB1r1/8 w - - 0 1");
  pinmask_t pinmask2 = generate_pinmask<state>(board2);
  EXPECT_EQ(pinmask2.hv, 0);
  EXPECT_EQ(pinmask2.d, 0);
}

TEST(pinmaskgen_rook, black_rook_two_blocker_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board2 = fen::parse("7K/8/8/8/8/8/1k1BB1r1/8 w - - 0 1")
                           .invertBoard();
  pinmask_t pinmask2 = generate_pinmask<state>(board2);
  EXPECT_EQ(pinmask2.hv, 0);
  EXPECT_EQ(pinmask2.d, 0);
}

TEST(pinmaskgen_rook, white_rook_two_blocker_up) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board3 = fen::parse("8/6k1/8/6B1/6P1/8/6r1/K7 w - - 0 1");
  pinmask_t pinmask3 = generate_pinmask<state>(board3);
  EXPECT_EQ(pinmask3.hv, 0);
  EXPECT_EQ(pinmask3.d, 0);
}

TEST(pinmaskgen_rook, black_rook_two_blocker_up) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board3 = fen::parse("8/6k1/8/6B1/6P1/8/6r1/K7 w - - 0 1")
                           .invertBoard();
  pinmask_t pinmask3 = generate_pinmask<state>(board3);
  EXPECT_EQ(pinmask3.hv, 0);
  EXPECT_EQ(pinmask3.d, 0);
}

TEST(pinmaskgen_rook, white_rook_two_blocker_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board4 = fen::parse("8/1r1PB1k1/8/8/8/8/8/K7 w - - 0 1");
  pinmask_t pinmask4 = generate_pinmask<state>(board4);
  EXPECT_EQ(pinmask4.hv, 0);
  EXPECT_EQ(pinmask4.d, 0);
}

TEST(pinmaskgen_rook, black_rook_two_blocker_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board4 = fen::parse("8/1r1PB1k1/8/8/8/8/8/K7 w - - 0 1")
                           .invertBoard();
  pinmask_t pinmask4 = generate_pinmask<state>(board4);
  EXPECT_EQ(pinmask4.hv, 0);
  EXPECT_EQ(pinmask4.d, 0);
}

