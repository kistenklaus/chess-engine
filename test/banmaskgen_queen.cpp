//
// Created by karl on 19/03/23.
//
#include <gtest/gtest.h>

#include "Board.h"
#include "banmask.h"
#include "banmask_generation.h"
#include "bitmap.h"
#include "fen.h"
#include "test_utils.h"

TEST(banmaskgen_queen, white_queen_no_blocker) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("4K3/7k/8/8/3q4/8/8/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 14432323891011267145ull, board);
}
TEST(banmaskgen_queen, white_queen_friendly_blocker) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("4K3/7k/1P1P1P2/8/1P1q1P2/8/1P1P1P2/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 13853329860679444992ull, board);
}

TEST(banmaskgen_queen, white_queen_enemy_blocker) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("4K3/7k/1r1b1r2/8/1b1q1b2/8/1r1b1r2/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 16318511221663858486ull, board);
}

TEST(banmaskgen_queen, black_queen_no_blocker) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("4K3/7k/8/8/3q4/8/8/8 w - - 0 1").invertBoard();
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 14432323891011267145ull, board);
}
TEST(banmaskgen_queen, black_queen_friendly_blocker) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse(
                          "4K3/7k/1P1P1P2/8/1P1q1P2/8/1P1P1P2/8 w - - "
                          "0 1")
                          .invertBoard();
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 13853329860679444992ull, board);
}

TEST(banmaskgen_queen, black_queen_enemy_blocker) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse(
                          "4K3/7k/1r1b1r2/8/1b1q1b2/8/1r1b1r2/8 w - - "
                          "0 1")
                          .invertBoard();
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 16318511221663858486ull, board);
}
