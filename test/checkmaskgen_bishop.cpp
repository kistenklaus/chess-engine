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

TEST(checkmaskgen_bishop, white_bishop_no_blocker_down_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1B6/8/8/8/8/6k1/K7 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 567382630203392, board);
}

TEST(checkmaskgen_bishop, black_bishop_no_blocker_down_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/1B6/8/8/8/8/6k1/K7 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 567382630203392, board);
}

TEST(checkmaskgen_bishop, white_bishop_no_blocker_down_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("K7/6B1/8/8/8/8/1k6/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 18049651735527424ull, board);
}

TEST(checkmaskgen_bishop, black_bishop_no_blocker_down_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("K7/6B1/8/8/8/8/1k6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 18049651735527424ull, board);
}

TEST(checkmaskgen_bishop, white_bishop_no_blocker_up_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("7K/1k6/8/8/8/8/6B1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 4432676798464, board);
}

TEST(checkmaskgen_bishop, black_bishop_no_blocker_up_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7K/1k6/8/8/8/8/6B1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 4432676798464, board);
}

TEST(checkmaskgen_bishop, white_bishop_no_blocker_up_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/6k1/8/8/8/8/1B6/7K w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 35253226045952ull, board);
}

TEST(checkmaskgen_bishop, black_bishop_no_blocker_up_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/6k1/8/8/8/8/1B6/7K w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 35253226045952ull, board);
}

TEST(checkmaskgen_bishop, white_bishop_one_friendly_blocker_down_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1B6/8/8/4P3/8/6k1/K7 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, black_bishop_one_friendly_blocker_down_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/1B6/8/8/4P3/8/6k1/K7 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, white_bishop_one_friendly_blocker_down_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("K7/6B1/8/8/3P4/8/1k6/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, black_bishop_one_friendly_blocker_down_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("K7/6B1/8/8/3P4/8/1k6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, white_bishop_one_friendly_blocker_up_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("7K/1k6/8/3P4/8/8/6B1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, black_bishop_one_friendly_blocker_up_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7K/1k6/8/3P4/8/8/6B1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, white_bishop_one_friendly_blocker_up_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/6k1/8/4P3/8/8/1B6/7K w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, black_bishop_one_friendly_blocker_up_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/6k1/8/4P3/8/8/1B6/7K w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, white_bishop_one_enemy_blocker_down_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/1B6/8/8/4p3/8/6k1/K7 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, black_bishop_one_enemy_blocker_down_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/1B6/8/8/4p3/8/6k1/K7 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, white_bishop_one_enemy_blocker_down_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("K7/6B1/8/8/3p4/8/1k6/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, black_bishop_one_enemy_blocker_down_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("K7/6B1/8/8/3p4/8/1k6/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, white_bishop_one_enemy_blocker_up_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("7K/1k6/8/3p4/8/8/6B1/8 w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, black_bishop_one_enemy_blocker_up_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7K/1k6/8/3p4/8/8/6B1/8 w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, white_bishop_one_enemy_blocker_up_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board =
      fen::parse("8/6k1/8/4p3/8/8/1B6/7K w - - 0 1").invertBoard();
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}

TEST(checkmaskgen_bishop, black_bishop_one_enemy_blocker_up_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/6k1/8/4p3/8/8/1B6/7K w - - 0 1");
  const checkmask_t checkmask = generate_checkmask<state>(board);
  EXPECT_BITBOARD(checkmask, 0xFFFFFFFFFFFFFFFF, board);
}
