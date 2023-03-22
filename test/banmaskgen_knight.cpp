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

TEST(banmaskgen_knight, black_knight_center) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("3K4/8/8/7k/2N5/8/8/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 1449044248238295552ull, board);
}

TEST(banmaskgen_knight, black_knight_right) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("3K4/8/8/8/k6N/8/8/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 1449103686291701760ull, board);
}

TEST(banmaskgen_knight, black_knight_bottom) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("3K4/8/8/8/k7/8/8/4N3 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 1449033180109095936ull, board);
}

TEST(banmaskgen_knight, black_knight_left) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("3K4/8/8/N6k/8/8/8/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 1449600528173629440ull, board);
}

TEST(banmaskgen_knight, black_knight_up) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("4N3/8/8/7k/8/2K5/8/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 19184279116975616ull, board);
}
