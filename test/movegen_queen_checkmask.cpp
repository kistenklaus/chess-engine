//
// Created by karl on 19/03/23.
//

#include <gtest/gtest.h>

#include "Board.h"
#include "GameState.h"
#include "MoveTestReceiver.h"
#include "bitmap.h"
#include "fen.h"
#include "move_generation.h"
#include "test_utils.h"


TEST(movegen_queen_checkmask, white_queen_legal_up_left) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/5K2/8/8/2b5/8/1k6/7Q w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 128, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 34359738368, board);
}

TEST(movegen_queen_checkmask, white_queen_legal_up_right) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/2K5/8/8/8/6b1/4k3/Q7 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 1, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 68719476736, board);
}

TEST(movegen_queen_checkmask, white_queen_legal_down_right) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("Q7/5K2/8/8/8/1b6/4k3/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 72057594037927936, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 34359738368, board);
}

TEST(movegen_queen_checkmask, white_queen_legal_down_left) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("7Q/8/1K6/8/6k1/8/5b2/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 9223372036854775808ull, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 134217728, board);
}

TEST(movegen_queen_checkmask, white_queen_legal_up) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("9/1K4r1/8/5k2/8/8/3Q4/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 2048, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 2251799813685248, board);
}

TEST(movegen_queen_checkmask, white_queen_legal_right) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/6K1/8/1Q6/8/4k3/6r1/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 8589934592, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 274877906944, board);
}

TEST(movegen_queen_checkmask, white_queen_legal_down) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/4Q3/8/8/2k5/8/1r4K1/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 4503599627370496, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 4096, board);
}

TEST(movegen_queen_checkmask, white_queen_legal_left) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1r6/3k4/8/6Q1/8/1K6/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 1073741824, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 33554432, board);
}


TEST(movegen_queen_checkmask, black_queen_legal_up_left) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/5K2/8/8/2b5/8/1k6/7Q w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 128, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 34359738368, board);
}

TEST(movegen_queen_checkmask, black_queen_legal_up_right) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/2K5/8/8/8/6b1/4k3/Q7 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 1, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 68719476736, board);
}

TEST(movegen_queen_checkmask, black_queen_legal_down_right) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("Q7/5K2/8/8/8/1b6/4k3/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 72057594037927936, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 34359738368, board);
}

TEST(movegen_queen_checkmask, black_queen_legal_down_left) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7Q/8/1K6/8/6k1/8/5b2/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 9223372036854775808ull, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 134217728, board);
}

TEST(movegen_queen_checkmask, black_queen_legal_up) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("9/1K4r1/8/5k2/8/8/3Q4/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 2048, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 2251799813685248, board);
}

TEST(movegen_queen_checkmask, black_queen_legal_right) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/6K1/8/1Q6/8/4k3/6r1/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 8589934592, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 274877906944, board);
}

TEST(movegen_queen_checkmask, black_queen_legal_down) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/4Q3/8/8/2k5/8/1r4K1/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 4503599627370496, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 4096, board);
}

TEST(movegen_queen_checkmask, black_queen_legal_left) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/1r6/3k4/8/6Q1/8/1K6/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.queenMoveCount(), 1);
  EXPECT_BITBOARD(receiver.queenOrigins(), 1073741824, board);
  EXPECT_BITBOARD(receiver.queenTargets(), 33554432, board);
}
