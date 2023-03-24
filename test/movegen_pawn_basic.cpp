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

TEST(movegen_pawn_basic, white_pawn_push) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/8/8/8/3P4/8/8/K7 w - - 0 1");
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 1);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 134217728, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 34359738368, board);
}

TEST(movegen_pawn_basic, white_pawn_double_push) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/8/8/8/8/8/3P4/K7 w - - 0 1");
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 2);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 2048, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 134742016, board);
}

TEST(movegen_pawn_basic, white_pawn_attack_left) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/8/8/8/8/2r5/3P4/K7 w - - 0 1");
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 3);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 2048, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 135004160, board);
}

TEST(movegen_pawn_basic, white_pawn_attack_right) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/8/8/8/8/4r3/3P4/K7 w - - 0 1");
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 3);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 2048, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 135790592, board);
}

TEST(movegen_pawn_basic, white_pawn_blocked_push) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/8/8/8/8/3r4/3P4/K7 w - - 0 1");
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 0);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 0, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 0, board);
}

TEST(movegen_pawn_basic, white_pawn_blocked_double_push) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/8/8/8/3r4/8/3P4/K7 w - - 0 1");
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 1);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 2048, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 524288, board);
}

TEST(movegen_pawn_basic, black_pawn_push) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/8/3p4/8/8/8/8/K7 w - - 0 1");
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 1);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 8796093022208, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 34359738368, board);
}

TEST(movegen_pawn_basic, black_pawn_double_push) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/3p4/8/8/8/8/8/K7 w - - 0 1");
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 2);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 2251799813685248, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 8830452760576, board);
}

TEST(movegen_pawn_basic, black_pawn_attack_left) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/3p4/2R5/8/8/8/8/K7 w - - 0 1");
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 3);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 2251799813685248, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 13228499271680, board);
}

TEST(movegen_pawn_basic, black_pawn_attack_right) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/3p4/4R3/8/8/8/8/K7 w - - 0 1");
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 3);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 2251799813685248, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 26422638804992, board);
}

TEST(movegen_pawn_basic, black_pawn_blocked_push) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/3p4/3R4/8/8/8/8/K7 w - - 0 1");
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 0);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 0, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 0, board);
}

TEST(movegen_pawn_basic, black_pawn_blocked_double_push) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7k/3p4/8/3R4/8/8/8/K7 w - - 0 1");
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 1);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 2251799813685248, board);
  EXPECT_BITBOARD(receiver.pawnTargets(), 8796093022208, board);
}