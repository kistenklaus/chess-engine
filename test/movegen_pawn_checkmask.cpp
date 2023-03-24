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

TEST(movegen_pawn_checkmask, white_pawn_legal_push) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("2k5/6b1/8/8/8/3P4/1K6/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 1);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 524288, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 134217728, board);
}

TEST(movegen_pawn_checkmask, white_pawn_illegal_push) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("2k5/6b1/8/8/8/4P3/1K6/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 0);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 0, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 0, board);
}

TEST(movegen_pawn_checkmask, white_pawn_legal_double_push) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("2k5/6b1/8/8/8/8/1K1P4/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 1);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 2048, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 134217728, board);
}

TEST(movegen_pawn_checkmask, white_pawn_illegal_double_push) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("2k5/6b1/8/8/8/8/1K2P3/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 0);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 0, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 0, board);
}

TEST(movegen_pawn_checkmask, black_pawn_legal_push) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7K/1k6/3p4/8/8/8/6B1/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 1);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 8796093022208ull, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 34359738368ull, board);
}

TEST(movegen_pawn_checkmask, black_pawn_illegal_push) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7K/1k6/4p3/8/8/8/6B1/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 0);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 0, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 0, board);
}

TEST(movegen_pawn_checkmask, black_pawn_legal_double_push) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7K/1k1p4/8/8/8/8/6B1/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 1);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 2251799813685248ull, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 34359738368ull, board);
}

TEST(movegen_pawn_checkmask, black_pawn_illegal_double_push) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7K/1k2p3/8/8/8/8/6B1/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 0);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 0, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 0, board);
}

