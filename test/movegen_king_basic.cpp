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

TEST(movegen_king_basic, white_king_center) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/8/8/3K4/8/8/8/8 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 8);
  EXPECT_BITBOARD(receiver.kingOrigins(), 34359738368, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 30872694685696, board);
}

TEST(movegen_king_basic, black_king_center) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/8/8/3K4/8/8/8/8 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 8);
  EXPECT_BITBOARD(receiver.kingOrigins(), 34359738368, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 30872694685696, board);
}

TEST(movegen_king_basic, white_king_top_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("K6k/8/8/8/8/8/8/8 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 3);
  EXPECT_BITBOARD(receiver.kingOrigins(), 72057594037927936, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 144959613005987840ull, board);
}

TEST(movegen_king_basic, black_king_top_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("K6k/8/8/8/8/8/8/8 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 3);
  EXPECT_BITBOARD(receiver.kingOrigins(), 72057594037927936, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 144959613005987840ull, board);
}

TEST(movegen_king_basic, white_king_top_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/8/8/8/8/8/8/8 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 3);
  EXPECT_BITBOARD(receiver.kingOrigins(), 9223372036854775808ull, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 4665729213955833856ull, board);
}

TEST(movegen_king_basic, black_king_top_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/8/8/8/8/8/8/8 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 3);
  EXPECT_BITBOARD(receiver.kingOrigins(), 9223372036854775808ull, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 4665729213955833856ull, board);
}

TEST(movegen_king_basic, white_king_bottom_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k7/8/8/8/8/8/8/7K w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 3);
  EXPECT_BITBOARD(receiver.kingOrigins(), 128, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 49216, board);
}

TEST(movegen_king_basic, black_king_bottom_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k7/8/8/8/8/8/8/7K w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 3);
  EXPECT_BITBOARD(receiver.kingOrigins(), 128, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 49216, board);
}

TEST(movegen_king_basic, white_king_bottom_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k7/8/8/8/8/8/8/K7 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 3);
  EXPECT_BITBOARD(receiver.kingOrigins(), 1, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 770, board);
}

TEST(movegen_king_basic, black_king_bottom_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k7/8/8/8/8/8/8/K7 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 3);
  EXPECT_BITBOARD(receiver.kingOrigins(), 1, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 770, board);
}

TEST(movegen_king_basic, white_king_blocked) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k7/8/3B4/3RK3/8/8/8/8 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 6);
  EXPECT_BITBOARD(receiver.kingOrigins(), 68719476736, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 52914936610816, board);
}

TEST(movegen_king_basic, black_king_blocked) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board =
      fen::parse("k7/8/3B4/3RK3/8/8/8/8 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 6);
  EXPECT_BITBOARD(receiver.kingOrigins(), 68719476736, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 52914936610816, board);
}
