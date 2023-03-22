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

TEST(checkmaskgen_rook, white_rook_no_blocker_left) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/5k2/8/8/8/8/1r4K1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 15872, board);
}

TEST(checkmaskgen_rook, white_rook_no_blocker_down) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1r6/8/8/8/6k1/1K6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 565157600296960, board);
}

TEST(checkmaskgen_rook, white_rook_no_blocker_right) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1K4r1/8/8/8/8/2k5/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 34902897112121344, board);
}

TEST(checkmaskgen_rook, white_rook_no_blocker_up) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/1k6/8/8/8/6r1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 70644700037120, board);
}

TEST(checkmaskgen_rook, white_rook_one_friendly_blocker_left) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/5k2/8/8/8/8/1r1P2K1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, white_rook_one_friendly_blocker_down) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1r6/8/8/1P6/6k1/1K6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, white_rook_one_friendly_blocker_right) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1K1P2r1/8/8/8/8/2k5/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, white_rook_one_friendly_blocker_up) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/1k6/8/6P1/8/6r1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, white_rook_one_enemy_blocker_left) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/5k2/8/8/8/8/1r1p2K1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, white_rook_one_enemy_blocker_down) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1r6/8/8/1p6/6k1/1K6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, white_rook_one_enemy_blocker_right) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1K1p2r1/8/8/8/8/2k5/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, white_rook_one_enemy_blocker_up) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/1k6/8/6p1/8/6r1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, black_rook_no_blocker_left) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/5k2/8/8/8/8/1r4K1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 15872, board);
}

TEST(checkmaskgen_rook, black_rook_no_blocker_down) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1r6/8/8/8/6k1/1K6/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 565157600296960, board);
}

TEST(checkmaskgen_rook, black_rook_no_blocker_right) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1K4r1/8/8/8/8/2k5/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 34902897112121344, board);
}

TEST(checkmaskgen_rook, black_rook_no_blocker_up) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/1k6/8/8/8/6r1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 70644700037120, board);
}

TEST(checkmaskgen_rook, black_rook_one_friendly_blocker_left) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/5k2/8/8/8/8/1r1P2K1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, black_rook_one_friendly_blocker_down) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1r6/8/8/1P6/6k1/1K6/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, black_rook_one_friendly_blocker_right) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1K1P2r1/8/8/8/8/2k5/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, black_rook_one_friendly_blocker_up) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/1k6/8/6P1/8/6r1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, black_rook_one_enemy_blocker_left) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/5k2/8/8/8/8/1r1p2K1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, black_rook_one_enemy_blocker_down) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1r6/8/8/1p6/6k1/1K6/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, black_rook_one_enemy_blocker_right) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/1K1p2r1/8/8/8/8/2k5/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_rook, black_rook_one_enemy_blocker_up) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board =
      fen::parse("8/6K1/1k6/8/6p1/8/6r1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}
