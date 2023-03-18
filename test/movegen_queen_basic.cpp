//
// Created by karl on 17/03/23.
//

#include <gtest/gtest.h>

#include "Board.h"
#include "MoveTestReceiver.h"
#include "bitmap.h"
#include "fen.h"
#include "move_generation.h"
#include "test_utils.h"

TEST(movegen_queen_basic, white_queen_no_blocker_center) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7K/8/8/3Q4/8/8/8/6k1 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 27);
  EXPECT_BITBOARD(receiver.queenOrigins(), 34359738368);
  EXPECT_BITBOARD(receiver.queenTargets(), 5272058161445620104);
}

TEST(movegen_queen_basic, black_queen_no_blocker_center) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board =
      fen::parse("7K/8/8/3Q4/8/8/8/6k1 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 27);
  EXPECT_BITBOARD(receiver.queenOrigins(), 34359738368);
  EXPECT_BITBOARD(receiver.queenTargets(), 5272058161445620104);
}

TEST(movegen_queen_basic, white_queen_no_blocker_left_border) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7K/8/8/Q7/8/8/8/7k w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 21);
  EXPECT_BITBOARD(receiver.queenOrigins(), 4294967296);
  EXPECT_BITBOARD(receiver.queenTargets(), 649930110732142865);
}

TEST(movegen_queen_basic, black_queen_no_blocker_left_border) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7K/8/8/Q7/8/8/8/7k w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 21);
  EXPECT_BITBOARD(receiver.queenOrigins(), 4294967296);
  EXPECT_BITBOARD(receiver.queenTargets(), 649930110732142865);
}

TEST(movegen_queen_basic, white_queen_no_blocker_top_border) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("3Q4/8/8/8/8/8/8/K6k w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 21);
  EXPECT_BITBOARD(receiver.queenOrigins(), 576460752303423488);
  EXPECT_BITBOARD(receiver.queenTargets(), 17806153522019305480ull);
}

TEST(movegen_queen_basic, black_queen_no_blocker_top_border) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("3Q4/8/8/8/8/8/8/K6k w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 21);
  EXPECT_BITBOARD(receiver.queenOrigins(), 576460752303423488);
  EXPECT_BITBOARD(receiver.queenTargets(), 17806153522019305480ull);
}

TEST(movegen_queen_basic, white_queen_no_blocker_right_border) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("1k6/8/8/7Q/8/8/8/K7 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 21);
  EXPECT_BITBOARD(receiver.queenOrigins(), 549755813888);
  EXPECT_BITBOARD(receiver.queenTargets(), 10421541192660455560ull);
}

TEST(movegen_queen_basic, black_queen_no_blocker_right_border) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("1k6/8/8/7Q/8/8/8/K7 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 21);
  EXPECT_BITBOARD(receiver.queenOrigins(), 549755813888);
  EXPECT_BITBOARD(receiver.queenTargets(), 10421541192660455560ull);
}

TEST(movegen_queen_basic, white_queen_no_blocker_bottom_border) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/8/8/8/8/8/8/3Q4 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 21);
  EXPECT_BITBOARD(receiver.queenOrigins(), 8);
  EXPECT_BITBOARD(receiver.queenTargets(), 578721933553179895ull);
}

TEST(movegen_queen_basic, black_queen_no_blocker_bottom_border) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/8/8/8/8/8/8/3Q4 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 21);
  EXPECT_BITBOARD(receiver.queenOrigins(), 8);
  EXPECT_BITBOARD(receiver.queenTargets(), 578721933553179895ull);
}

TEST(movegen_queen_basic, white_queen_friendly_blocker_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/8/8/8/2Q2P2/8/8/8 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 22);
  EXPECT_BITBOARD(receiver.queenOrigins(), 67108864);
  EXPECT_BITBOARD(receiver.queenTargets(), 4910072644068316452);
}

TEST(movegen_queen_basic, black_queen_friendly_blocker_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board =
      fen::parse("k6K/8/8/8/2Q2P2/8/8/8 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 22);
  EXPECT_BITBOARD(receiver.queenOrigins(), 67108864);
  EXPECT_BITBOARD(receiver.queenTargets(), 4910072644068316452);
}

TEST(movegen_queen_basic, white_queen_friendly_blocker_top) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/2P5/8/8/2Q5/8/8/8 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 23);
  EXPECT_BITBOARD(receiver.queenOrigins(), 67108864);
  EXPECT_BITBOARD(receiver.queenTargets(), 4620716371767858468ull);
}

TEST(movegen_queen_basic, black_queen_friendly_blocker_top) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board =
      fen::parse("k6K/2P5/8/8/2Q5/8/8/8 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 23);
  EXPECT_BITBOARD(receiver.queenOrigins(), 67108864);
  EXPECT_BITBOARD(receiver.queenTargets(), 4620716371767858468ull);
}

TEST(movegen_queen_basic, white_queen_friendly_blocker_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/8/8/2P2Q2/8/8/8/8 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 22);
  EXPECT_BITBOARD(receiver.queenOrigins(), 137438953472);
  EXPECT_BITBOARD(receiver.queenTargets(), 2641485256358110242ull);
}

TEST(movegen_queen_basic, black_queen_friendly_blocker_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board =
      fen::parse("k6K/8/8/2P2Q2/8/8/8/8 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 22);
  EXPECT_BITBOARD(receiver.queenOrigins(), 137438953472);
  EXPECT_BITBOARD(receiver.queenTargets(), 2641485256358110242ull);
}

TEST(movegen_queen_basic, white_queen_friendly_blocker_bottom) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/8/8/5Q2/8/8/5P2/8 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 23);
  EXPECT_BITBOARD(receiver.queenOrigins(), 137438953472);
  EXPECT_BITBOARD(receiver.queenTargets(), 2641485286422873090ull);
}

TEST(movegen_queen_basic, black_queen_friendly_blocker_bottom) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board =
      fen::parse("k6K/8/8/5Q2/8/8/5P2/8 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 23);
  EXPECT_BITBOARD(receiver.queenOrigins(), 137438953472);
  EXPECT_BITBOARD(receiver.queenTargets(), 2641485286422873090ull);
}

// DIAGONAL BLOCKERS

TEST(movegen_queen_basic, white_queen_friendly_blocker_up_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("7K/8/8/3P4/8/8/8/k6Q w - - 0 1");
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 16);
  EXPECT_BITBOARD(receiver.queenOrigins(), 128);
  EXPECT_BITBOARD(receiver.queenTargets(), 36170086689587327);
}

TEST(movegen_queen_basic, black_queen_friendly_blocker_up_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7K/8/8/3P4/8/8/8/k6Q w - - 0 1").invertBoard();
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 16);
  EXPECT_BITBOARD(receiver.queenOrigins(), 128);
  EXPECT_BITBOARD(receiver.queenTargets(), 36170086689587327);
}

TEST(movegen_queen_basic, white_queen_friendly_blocker_up_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k7/8/8/4P3/8/8/8/Q6K w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 16);
  EXPECT_BITBOARD(receiver.queenOrigins(), 1);
  EXPECT_BITBOARD(receiver.queenTargets(), 72340172972557182);
}

TEST(movegen_queen_basic, black_queen_friendly_blocker_up_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k7/8/8/4P3/8/8/8/Q6K w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 16);
  EXPECT_BITBOARD(receiver.queenOrigins(), 1);
  EXPECT_BITBOARD(receiver.queenTargets(), 72340172972557182);
}

TEST(movegen_queen_basic, white_queen_friendly_blocker_down_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("Q7/8/8/8/4P3/8/3k4/7K w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 17);
  EXPECT_BITBOARD(receiver.queenOrigins(), 72057594037927936);
  EXPECT_BITBOARD(receiver.queenTargets(), 18303478846793515265ull);
}

TEST(movegen_queen_basic, black_queen_friendly_blocker_down_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("Q7/8/8/8/4P3/8/3k4/7K w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 17);
  EXPECT_BITBOARD(receiver.queenOrigins(), 72057594037927936);
  EXPECT_BITBOARD(receiver.queenTargets(), 18303478846793515265ull);
}

TEST(movegen_queen_basic, white_queen_friendly_blocker_down_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7Q/8/8/8/3P4/8/3k4/7K w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 16);
  EXPECT_BITBOARD(receiver.queenOrigins(), 9223372036854775808ull);
  EXPECT_BITBOARD(receiver.queenTargets(), 9205534180836933632ull);
}

TEST(movegen_queen_basic, black_queen_friendly_blocker_down_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7Q/8/8/8/3P4/8/3k4/7K w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 16);
  EXPECT_BITBOARD(receiver.queenOrigins(), 9223372036854775808ull);
  EXPECT_BITBOARD(receiver.queenTargets(), 9205534180836933632ull);
}

TEST(movegen_queen_basic, white_queen_enemy_blocker_up_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7K/8/8/3p4/8/8/8/k6Q w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 17);
  EXPECT_BITBOARD(receiver.queenOrigins(), 128);
  EXPECT_BITBOARD(receiver.queenTargets(), 36170121049325695);
}

TEST(movegen_queen_basic, black_queen_enemy_blocker_up_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7K/8/8/3p4/8/8/8/k6Q w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 17);
  EXPECT_BITBOARD(receiver.queenOrigins(), 128);
  EXPECT_BITBOARD(receiver.queenTargets(), 36170121049325695);
}


TEST(movegen_queen_basic, white_queen_enemy_blocker_up_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k7/8/8/4p3/8/8/8/Q6K w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 17);
  EXPECT_BITBOARD(receiver.queenOrigins(), 1);
  EXPECT_BITBOARD(receiver.queenTargets(), 72340241692033918);
}

TEST(movegen_queen_basic, black_queen_enemy_blocker_up_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k7/8/8/4p3/8/8/8/Q6K w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 17);
  EXPECT_BITBOARD(receiver.queenOrigins(), 1);
  EXPECT_BITBOARD(receiver.queenTargets(), 72340241692033918);
}

TEST(movegen_queen_basic, white_queen_enemy_blocker_down_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("Q7/8/8/8/4p3/8/3k4/7K w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 18);
  EXPECT_BITBOARD(receiver.queenOrigins(), 72057594037927936);
  EXPECT_BITBOARD(receiver.queenTargets(), 18303478847061950721ull);
}

TEST(movegen_queen_basic, black_queen_enemy_blocker_down_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("Q7/8/8/8/4p3/8/3k4/7K w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 18);
  EXPECT_BITBOARD(receiver.queenOrigins(), 72057594037927936);
  EXPECT_BITBOARD(receiver.queenTargets(), 18303478847061950721ull);
}

TEST(movegen_queen_basic, white_queen_enemy_blocker_down_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7Q/8/8/8/3p4/8/3k4/7K w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 17);
  EXPECT_BITBOARD(receiver.queenOrigins(), 9223372036854775808ull);
  EXPECT_BITBOARD(receiver.queenTargets(), 9205534180971151360);
}

TEST(movegen_queen_basic, black_queen_enemy_blocker_down_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7Q/8/8/8/3p4/8/3k4/7K w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 17);
  EXPECT_BITBOARD(receiver.queenOrigins(), 9223372036854775808ull);
  EXPECT_BITBOARD(receiver.queenTargets(), 9205534180971151360);
}
