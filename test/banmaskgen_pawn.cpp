//
// Created by karl on 18/03/23.
//

#include <gtest/gtest.h>

#include "Board.h"
#include "bitmap.h"
#include "banmask.h"
#include "fen.h"
#include "test_utils.h"
#include "banmask_generation.h"

TEST(banmaskgen_pawn, white_pawns_center) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("3k4/8/8/3p4/8/8/8/3K4 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 1449033180442001408ull, board);
}

TEST(banmaskgen_pawn, white_pawns_center_occupied) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("3k4/8/8/3p4/2P1p3/8/8/3K4 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 1449033180444622848ull, board);
}

TEST(banmaskgen_pawn, white_pawns_center_blocked) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("3k4/8/8/3p4/3P4/8/8/3K4 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 1449033180442001408ull, board);
}

TEST(banmaskgen_pawn, black_pawns_center) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("3k4/8/8/8/3P4/8/8/3K4 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 85899353108, board);
}

TEST(banmaskgen_pawn, black_pawns_center_occupied) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("3k4/8/8/2p1p3/3P4/8/8/3K4 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 85899353108, board);
}

TEST(banmaskgen_pawn, black_pawns_center_blocked) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("3k4/8/8/3p4/3P4/8/8/3K4 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 85899353108, board);
}
