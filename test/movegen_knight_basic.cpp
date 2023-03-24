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

TEST(movegen_knight_basic, white_knight_center) {
  const Board board1 = fen::parse("7k/8/8/3N4/8/8/8/K7 w - - 0 1");
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  enumerateMoves<state>(board1, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 8);
  EXPECT_BITBOARD(receiver.knightOrigins(), 34359738368, board1);
  EXPECT_BITBOARD(receiver.knightTargets(), 5666883501293568, board1);
}

TEST(movegen_knight_basic, black_knight_center) {
  const Board board1 =
      fen::parse("7k/8/8/3N4/8/8/8/K7 w - - 0 1").invertBoard();
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  enumerateMoves<state>(board1, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 8);
  EXPECT_BITBOARD(receiver.knightOrigins(), 34359738368, board1);
  EXPECT_BITBOARD(receiver.knightTargets(), 5666883501293568, board1);
}

TEST(movegen_knight_basic, white_knight_bottom_right) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board1 = fen::parse("7k/8/8/8/8/8/8/K6N w - - 0 1");
  enumerateMoves<state>(board1, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_BITBOARD(receiver.knightOrigins(), 128, board1);
  EXPECT_BITBOARD(receiver.knightTargets(), 4202496, board1);
}

TEST(movegen_knight_basic, black_knight_bottom_right) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board1 = fen::parse("7k/8/8/8/8/8/8/K6N w - - 0 1").invertBoard();
  enumerateMoves<state>(board1, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_BITBOARD(receiver.knightOrigins(), 128, board1);
  EXPECT_BITBOARD(receiver.knightTargets(), 4202496, board1);
}

TEST(movegen_knight_basic, white_knight_top_right) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board2 = fen::parse("k6N/8/8/8/8/8/8/K7 w - - 0 1");
  enumerateMoves<state>(board2, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_BITBOARD(receiver.knightOrigins(), 9223372036854775808ull, board2);
  EXPECT_BITBOARD(receiver.knightTargets(), 9077567998918656, board2);
}

TEST(movegen_knight_basic, black_knight_top_right) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board2 = fen::parse("k6N/8/8/8/8/8/8/K7 w - - 0 1").invertBoard();
  enumerateMoves<state>(board2, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_BITBOARD(receiver.knightOrigins(), 9223372036854775808ull, board2);
  EXPECT_BITBOARD(receiver.knightTargets(), 9077567998918656, board2);
}

TEST(movegen_knight_basic, white_knight_top_left) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board3 = fen::parse("N7/8/8/8/8/8/8/K6k w - - 0 1");
  enumerateMoves<state>(board3, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_BITBOARD(receiver.knightOrigins(), 72057594037927936, board3);
  EXPECT_BITBOARD(receiver.knightTargets(), 1128098930098176, board3);
}

TEST(movegen_knight_basic, black_knight_top_left) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board3 = fen::parse("N7/8/8/8/8/8/8/K6k w - - 0 1").invertBoard();
  enumerateMoves<state>(board3, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_BITBOARD(receiver.knightOrigins(), 72057594037927936, board3);
  EXPECT_BITBOARD(receiver.knightTargets(), 1128098930098176, board3);
}

TEST(movegen_knight_basic, white_knight_bottom_left) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board4 = fen::parse("7K/8/8/8/8/8/8/N6k w - - 0 1");
  enumerateMoves<state>(board4, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_BITBOARD(receiver.knightOrigins(), 1, board4);
  EXPECT_BITBOARD(receiver.knightTargets(), 132096, board4);
}

TEST(movegen_knight_basic, black_knight_bottom_left) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board4 = fen::parse("7K/8/8/8/8/8/8/N6k w - - 0 1").invertBoard();
  enumerateMoves<state>(board4, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_BITBOARD(receiver.knightOrigins(), 1, board4);
  EXPECT_BITBOARD(receiver.knightTargets(), 132096, board4);
}
