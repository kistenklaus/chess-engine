//
// Created by karl on 18/03/23.
//

#include <gtest/gtest.h>

#include "Board.h"
#include "bitmap.h"
#include "checkmask.h"
#include "checkmask_generation.h"
#include "fen.h"
#include "test_utils.h"

TEST(checkmaskgen_knight, white_knight_no_attack) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/8/8/2K3k1/8/8/5n2/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF);
}

TEST(checkmaskgen_knight, black_knight_no_attack) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/8/8/2K3k1/8/8/5n2/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF);
}

TEST(checkmaskgen_knight, white_knight_attack) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/8/8/2K3k1/8/3n4/8/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 524288);
}

TEST(checkmaskgen_knight, black_knight_attack) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/8/8/2K3k1/8/3n4/8/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 524288);
}
