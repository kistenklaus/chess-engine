//
// Created by karl on 18/03/23.
//
#include "test_utils.h"

#include <gtest/gtest.h>

#include "Board.h"
#include "bitmap.h"
#include "fen.h"
#include "checkmask_generation.h"
#include "checkmask.h"

TEST(checkmaskgen_pawn, white_pawn_no_attack) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("k7/8/8/8/3P4/8/1K6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFFull);
}

TEST(checkmaskgen_pawn, black_pawn_no_attack) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/5k2/8/8/3p4/8/1K6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFFull);
}

TEST(checkmaskgen_pawn, white_pawn_attack_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/8/8/2k5/3P4/8/1K6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0x8000000);
}

TEST(checkmaskgen_pawn, black_pawn_attack_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/5k2/8/8/3p4/2K5/8/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0x8000000);
}

TEST(checkmaskgen_pawn, white_pawn_attack_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/8/8/4k3/3P4/8/1K6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0x8000000);
}

TEST(checkmaskgen_pawn, black_pawn_attack_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/5k2/8/8/3p4/4K3/8/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0x8000000);
}

TEST(checkmaskgen_pawn, white_pawn_blocked) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/8/8/3k4/3P4/8/1K6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFFull);
}

TEST(checkmaskgen_pawn, black_pawn_blocked) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/8/8/6k1/3p4/3K4/8/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFFull);
}

