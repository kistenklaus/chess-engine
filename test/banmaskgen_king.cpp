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

TEST(banmaskgen_king, black_king_center) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("3k4/8/8/8/4K3/8/8/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 241192927232, board);
}

TEST(banmaskgen_king, black_king_right) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("3k4/8/8/8/7K/8/8/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 825720045568, board);
}

TEST(banmaskgen_king, black_king_bottom) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("3k4/8/8/8/8/8/8/4K3 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 14376, board);
}

TEST(banmaskgen_king, black_king_left) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("3k4/8/8/K7/8/8/8/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 3307175149568, board);
}

TEST(banmaskgen_king, black_king_up) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("3K4/8/8/7k/8/8/8/8 w - - 0 1");
  const banmask_t banmask = generate_banmask<state>(board);
  EXPECT_BITBOARD(banmask, 1449033180106457088ull, board);
}
