//
// Created by karl on 19/03/23.
//

#include <gtest/gtest.h>

#include "Board.h"
#include "BoardState.h"
#include "MoveTestReceiver.h"
#include "bitmap.h"
#include "fen.h"
#include "move_generation.h"
#include "test_utils.h"

TEST(movegen_bishop, white_bishop_legal_up_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/5K2/8/8/2b5/8/1k6/7B w - - 0 1");
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 1);
  EXPECT_BITBOARD(receiver.bishopOrigins(), 128, board);
  EXPECT_BITBOARD(receiver.bishopTargets(), 34359738368, board);
}

TEST(movegen_bishop, white_bishop_legal_up_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/2K5/8/8/8/6b1/4k3/B7 w - - 0 1");
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 1);
  EXPECT_BITBOARD(receiver.bishopOrigins(), 1, board);
  EXPECT_BITBOARD(receiver.bishopTargets(), 68719476736, board);
}

TEST(movegen_bishop, white_bishop_legal_down_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("B7/5K2/8/8/8/1b6/4k3/8 w - - 0 1");
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 1);
  EXPECT_BITBOARD(receiver.bishopOrigins(), 72057594037927936, board);
  EXPECT_BITBOARD(receiver.bishopTargets(), 34359738368, board);
}

TEST(movegen_bishop, white_bishop_legal_down_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("7B/8/1K6/8/6k1/8/5b2/8 w - - 0 1");
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 1);
  EXPECT_BITBOARD(receiver.bishopOrigins(), 9223372036854775808ull, board);
  EXPECT_BITBOARD(receiver.bishopTargets(), 134217728, board);
}

TEST(movegen_bishop, black_bishop_legal_up_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/5K2/8/8/2b5/8/1k6/7B w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 1);
  EXPECT_BITBOARD(receiver.bishopOrigins(), 128, board);
  EXPECT_BITBOARD(receiver.bishopTargets(), 34359738368, board);
}

TEST(movegen_bishop, black_bishop_legal_up_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/2K5/8/8/8/6b1/4k3/B7 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 1);
  EXPECT_BITBOARD(receiver.bishopOrigins(), 1, board);
  EXPECT_BITBOARD(receiver.bishopTargets(), 68719476736, board);
}

TEST(movegen_bishop, black_bishop_legal_down_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("B7/5K2/8/8/8/1b6/4k3/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 1);
  EXPECT_BITBOARD(receiver.bishopOrigins(), 72057594037927936, board);
  EXPECT_BITBOARD(receiver.bishopTargets(), 34359738368, board);
}

TEST(movegen_bishop, black_bishop_legal_down_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7B/8/1K6/8/6k1/8/5b2/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 1);
  EXPECT_BITBOARD(receiver.bishopOrigins(), 9223372036854775808ull, board);
  EXPECT_BITBOARD(receiver.bishopTargets(), 134217728, board);
}
