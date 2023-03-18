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

TEST(checkmaskgen_queen, white_queen_no_blocker_down_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1B6/8/8/8/8/6k1/K7 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 567382630203392, board);
}

TEST(checkmaskgen_queen, black_queen_no_blocker_down_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/1B6/8/8/8/8/6k1/K7 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 567382630203392, board);
}

TEST(checkmaskgen_queen, white_queen_no_blocker_down_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("K7/6B1/8/8/8/8/1k6/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 18049651735527424ull, board);
}

TEST(checkmaskgen_queen, black_queen_no_blocker_down_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("K7/6B1/8/8/8/8/1k6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 18049651735527424ull, board);
}

TEST(checkmaskgen_queen, white_queen_no_blocker_up_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("7K/1k6/8/8/8/8/6B1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 4432676798464, board);
}

TEST(checkmaskgen_queen, black_queen_no_blocker_up_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7K/1k6/8/8/8/8/6B1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 4432676798464, board);
}

TEST(checkmaskgen_queen, white_queen_no_blocker_up_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/6k1/8/8/8/8/1B6/7K w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 35253226045952ull, board);
}

TEST(checkmaskgen_queen, black_queen_no_blocker_up_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/6k1/8/8/8/8/1B6/7K w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 35253226045952ull, board);
}

TEST(checkmaskgen_queen, white_queen_one_friendly_blocker_down_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1B6/8/8/4P3/8/6k1/K7 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_friendly_blocker_down_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/1B6/8/8/4P3/8/6k1/K7 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_friendly_blocker_down_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("K7/6B1/8/8/3P4/8/1k6/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_friendly_blocker_down_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("K7/6B1/8/8/3P4/8/1k6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_friendly_blocker_up_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("7K/1k6/8/3P4/8/8/6B1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_friendly_blocker_up_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7K/1k6/8/3P4/8/8/6B1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_friendly_blocker_up_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/6k1/8/4P3/8/8/1B6/7K w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_friendly_blocker_up_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/6k1/8/4P3/8/8/1B6/7K w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_enemy_blocker_down_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1B6/8/8/4p3/8/6k1/K7 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_enemy_blocker_down_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/1B6/8/8/4p3/8/6k1/K7 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_enemy_blocker_down_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("K7/6B1/8/8/3p4/8/1k6/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_enemy_blocker_down_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("K7/6B1/8/8/3p4/8/1k6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_enemy_blocker_up_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("7K/1k6/8/3p4/8/8/6B1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_enemy_blocker_up_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7K/1k6/8/3p4/8/8/6B1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_enemy_blocker_up_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/6k1/8/4p3/8/8/1B6/7K w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_enemy_blocker_up_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/6k1/8/4p3/8/8/1B6/7K w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_no_blocker_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/5k2/8/8/8/8/1r4K1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 15872, board);
}

TEST(checkmaskgen_queen, white_queen_no_blocker_down) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1r6/8/8/8/6k1/1K6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 565157600296960, board);
}

TEST(checkmaskgen_queen, white_queen_no_blocker_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1K4r1/8/8/8/8/2k5/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 34902897112121344, board);
}

TEST(checkmaskgen_queen, white_queen_no_blocker_up) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/1k6/8/8/8/6r1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 70644700037120, board);
}

TEST(checkmaskgen_queen, white_queen_one_friendly_blocker_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/5k2/8/8/8/8/1r1P2K1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_friendly_blocker_down) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1r6/8/8/1P6/6k1/1K6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_friendly_blocker_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1K1P2r1/8/8/8/8/2k5/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_friendly_blocker_up) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/1k6/8/6P1/8/6r1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_enemy_blocker_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/5k2/8/8/8/8/1r1p2K1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_enemy_blocker_down) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1r6/8/8/1p6/6k1/1K6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_enemy_blocker_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1K1p2r1/8/8/8/8/2k5/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, white_queen_one_enemy_blocker_up) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/1k6/8/6p1/8/6r1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_no_blocker_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/5k2/8/8/8/8/1r4K1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 15872, board);
}

TEST(checkmaskgen_queen, black_queen_no_blocker_down) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1r6/8/8/8/6k1/1K6/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 565157600296960, board);
}

TEST(checkmaskgen_queen, black_queen_no_blocker_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1K4r1/8/8/8/8/2k5/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 34902897112121344, board);
}

TEST(checkmaskgen_queen, black_queen_no_blocker_up) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/1k6/8/8/8/6r1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 70644700037120, board);
}

TEST(checkmaskgen_queen, black_queen_one_friendly_blocker_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/5k2/8/8/8/8/1r1P2K1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_friendly_blocker_down) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1r6/8/8/1P6/6k1/1K6/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_friendly_blocker_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1K1P2r1/8/8/8/8/2k5/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_friendly_blocker_up) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/1k6/8/6P1/8/6r1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_enemy_blocker_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/5k2/8/8/8/8/1r1p2K1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_enemy_blocker_down) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1r6/8/8/1p6/6k1/1K6/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_enemy_blocker_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1K1p2r1/8/8/8/8/2k5/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_queen, black_queen_one_enemy_blocker_up) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/1k6/8/6p1/8/6r1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}
