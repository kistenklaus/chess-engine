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

TEST(movegen_rook_checkmask, white_rook_legal_up) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1K4r1/8/5k2/8/8/3R4/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 1);
  EXPECT_BITBOARD(receiver.rookOrigins(), 2048, board);
  EXPECT_BITBOARD(receiver.rookTargets(), 2251799813685248, board);
}

TEST(movegen_rook_checkmask, white_rook_legal_right) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/6K1/8/1R6/8/4k3/6r1/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 1);
  EXPECT_BITBOARD(receiver.rookOrigins(), 8589934592, board);
  EXPECT_BITBOARD(receiver.rookTargets(), 274877906944, board);
}

TEST(movegen_rook_checkmask, white_rook_legal_down) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/4R3/8/8/2k5/8/1r4K1/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 1);
  EXPECT_BITBOARD(receiver.rookOrigins(), 4503599627370496, board);
  EXPECT_BITBOARD(receiver.rookTargets(), 4096, board);
}

TEST(movegen_rook_checkmask, white_rook_legal_left) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1r6/3k4/8/6R1/8/1K6/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 1);
  EXPECT_BITBOARD(receiver.rookOrigins(), 1073741824, board);
  EXPECT_BITBOARD(receiver.rookTargets(), 33554432, board);
}

TEST(movegen_rook_checkmask, black_rook_legal_up) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/1K4r1/8/5k2/8/8/3R4/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 1);
  EXPECT_BITBOARD(receiver.rookOrigins(), 2048, board);
  EXPECT_BITBOARD(receiver.rookTargets(), 2251799813685248, board);
}

TEST(movegen_rook_checkmask, black_rook_legal_right) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/6K1/8/1R6/8/4k3/6r1/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 1);
  EXPECT_BITBOARD(receiver.rookOrigins(), 8589934592, board);
  EXPECT_BITBOARD(receiver.rookTargets(), 274877906944, board);
}

TEST(movegen_rook_checkmask, black_rook_legal_down) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/4R3/8/8/2k5/8/1r4K1/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 1);
  EXPECT_BITBOARD(receiver.rookOrigins(), 4503599627370496, board);
  EXPECT_BITBOARD(receiver.rookTargets(), 4096, board);
}

TEST(movegen_rook_checkmask, black_rook_legal_left) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/1r6/3k4/8/6R1/8/1K6/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 1);
  EXPECT_BITBOARD(receiver.rookOrigins(), 1073741824, board);
  EXPECT_BITBOARD(receiver.rookTargets(), 33554432, board);
}
