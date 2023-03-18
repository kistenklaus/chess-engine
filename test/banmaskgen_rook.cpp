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

TEST(banmaskgen_rook, white_rook_no_blocker) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("4K3/7k/8/8/3r4/8/8/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 14432004946738546696ull, board);
}

TEST(banmaskgen_rook, white_rook_one_friendly_blocker) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("4K3/7k/3B4/8/1B1r1B2/8/3B4/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 13853292391383435264ull, board);
}

TEST(banmaskgen_rook, white_rook_one_enemy_blocker) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("4K3/7k/3b4/8/1b1r1b2/8/3b4/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 16308880453285021716ull, board);
}

TEST(banmaskgen_rook, black_rook_no_blocker) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("4K3/7k/8/8/3r4/8/8/8 w - - 0 1").invertBoard();
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 14432004946738546696ull, board);
}

TEST(banmaskgen_rook, black_rook_one_friendly_blocker) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("4K3/7k/3B4/8/1B1r1B2/8/3B4/8 w - - 0 1").invertBoard();
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 13853292391383435264ull, board);
}

TEST(banmaskgen_rook, black_rook_one_enemy_blocker) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("4K3/7k/3b4/8/1b1r1b2/8/3b4/8 w - - 0 1").invertBoard();
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 16308880453285021716ull, board);
}
