//
// Created by karl on 19/03/23.
//

#include <gtest/gtest.h>

#include "Board.h"
#include "bitmap.h"
#include "banmask.h"
#include "fen.h"
#include "test_utils.h"
#include "banmask_generation.h"

TEST(banmaskgen_bishop, white_bishop_no_blocker) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("4K3/8/8/7k/3b4/8/8/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 9241916763968643649ull, board);
}

TEST(banmaskgen_bishop, white_bishop_one_friendly_blocker) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("4K3/8/1P3P2/7k/3b4/8/1P3P2/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 0xE254C0142200, board);
}

TEST(banmaskgen_bishop, white_bishop_one_enemy_blocker) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("4K3/8/1r3r2/7k/3b4/8/1r3r2/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 2459809232564191010ull, board);
}

TEST(banmaskgen_bishop, black_bishop_no_blocker) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("4K3/8/8/7k/3b4/8/8/8 w - - 0 1").invertBoard();
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 9241916763968643649ull, board);
}

TEST(banmaskgen_bishop, black_bishop_one_friendly_blocker) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("4K3/8/1P3P2/7k/3b4/8/1P3P2/8 w - - 0 1").invertBoard();
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 0xE254C0142200, board);
}

TEST(banmaskgen_bishop, black_bishop_one_enemy_blocker) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("4K3/8/1r3r2/7k/3b4/8/1r3r2/8 w - - 0 1").invertBoard();
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 2459809232564191010ull, board);
}

