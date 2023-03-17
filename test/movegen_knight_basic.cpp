//
// Created by karl on 17/03/23.
//

#include <gtest/gtest.h>

#include "Board.h"
#include "MoveTestReceiver.h"
#include "bitmap.h"
#include "fen.h"
#include "move_generation.h"

TEST(movegen_knight_basic, white_knight_center) {
  const Board board1 = fen::parse("7k/8/8/3N4/8/8/8/K7 w - - 0 1");

  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);

  MoveTestReceiver receiver;
  generate_moves<state>(board1, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 8);
  EXPECT_EQ(receiver.knightOrigins(), 34359738368);
  EXPECT_EQ(receiver.knightTargets(), 5666883501293568);
}

TEST(movegen_knight_basic, black_knight_center) {
  const Board board1 =
      fen::parse("7k/8/8/3N4/8/8/8/K7 w - - 0 1").invertBoard();

  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);

  MoveTestReceiver receiver;
  generate_moves<state>(board1, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 8);
  EXPECT_EQ(receiver.knightOrigins(), 34359738368);
  EXPECT_EQ(receiver.knightTargets(), 5666883501293568);
}

TEST(movegen_knight_basic, white_knight_bottom_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board1 = fen::parse("7k/8/8/8/8/8/8/K6N w - - 0 1");
  generate_moves<state>(board1, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_EQ(receiver.knightOrigins(), 128);
  EXPECT_EQ(receiver.knightTargets(), 4202496);
}

TEST(movegen_knight_basic, black_knight_bottom_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board1 = fen::parse("7k/8/8/8/8/8/8/K6N w - - 0 1").invertBoard();
  generate_moves<state>(board1, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_EQ(receiver.knightOrigins(), 128);
  EXPECT_EQ(receiver.knightTargets(), 4202496);
}

TEST(movegen_knight_basic, white_knight_top_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board2 = fen::parse("k6N/8/8/8/8/8/8/K7 w - - 0 1");
  generate_moves<state>(board2, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_EQ(receiver.knightOrigins(), 9223372036854775808ull);
  EXPECT_EQ(receiver.knightTargets(), 9077567998918656);
}

TEST(movegen_knight_basic, black_knight_top_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board2 = fen::parse("k6N/8/8/8/8/8/8/K7 w - - 0 1").invertBoard();
  generate_moves<state>(board2, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_EQ(receiver.knightOrigins(), 9223372036854775808ull);
  EXPECT_EQ(receiver.knightTargets(), 9077567998918656);
}

TEST(movegen_knight_basic, white_knight_top_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board3 = fen::parse("N7/8/8/8/8/8/8/K6k w - - 0 1");
  generate_moves<state>(board3, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_EQ(receiver.knightOrigins(), 72057594037927936);
  EXPECT_EQ(receiver.knightTargets(), 1128098930098176);
}

TEST(movegen_knight_basic, black_knight_top_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board3 = fen::parse("N7/8/8/8/8/8/8/K6k w - - 0 1").invertBoard();
  generate_moves<state>(board3, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_EQ(receiver.knightOrigins(), 72057594037927936);
  EXPECT_EQ(receiver.knightTargets(), 1128098930098176);
}

TEST(movegen_knight_basic, white_knight_bottom_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board4 = fen::parse("7K/8/8/8/8/8/8/N6k w - - 0 1");
  generate_moves<state>(board4, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_EQ(receiver.knightOrigins(), 1);
  EXPECT_EQ(receiver.knightTargets(), 132096);
}

TEST(movegen_knight_basic, black_knight_bottom_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board4 = fen::parse("7K/8/8/8/8/8/8/N6k w - - 0 1").invertBoard();
  generate_moves<state>(board4, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_EQ(receiver.knightOrigins(), 1);
  EXPECT_EQ(receiver.knightTargets(), 132096);
}
