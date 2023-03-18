//
// Created by karl on 17/03/23.
//
#include <gtest/gtest.h>
#include "test_utils.h"

#include "Board.h"
#include "MoveTestReceiver.h"
#include "bitmap.h"
#include "fen.h"
#include "move_generation.h"

TEST(movegen_common_basic, white_starting_position) {
  const Board board =
      fen::parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  MoveTestReceiver receiver;
  generate_moves<BoardState::Default()>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 16);
  EXPECT_BITBOARD(receiver.pawnTargets(), 0xFFFF0000);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 0xFF00);
  EXPECT_EQ(receiver.bishopMoveCount(), 0);
  EXPECT_EQ(receiver.knightMoveCount(), 4);
  EXPECT_BITBOARD(receiver.knightTargets(), 0xA50000);
  EXPECT_BITBOARD(receiver.knightOrigins(), 0x42);
  EXPECT_EQ(receiver.rookMoveCount(), 0);
  EXPECT_EQ(receiver.queenMoveCount(), 0);
  EXPECT_EQ(receiver.kingMoveCount(), 0);
}

TEST(movegen_common_basic, black_starting_position) {
  const Board board =
      fen::parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  constexpr BoardState state = BoardState(BLACK, false, true, true, true, true);
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.pawnMoveCount(), 16);
  EXPECT_BITBOARD(receiver.pawnTargets(), 0xFFFF00000000);
  EXPECT_BITBOARD(receiver.pawnOrigins(), 0xFF000000000000);
  EXPECT_EQ(receiver.bishopMoveCount(), 0);
  EXPECT_EQ(receiver.knightMoveCount(), 4);
  EXPECT_BITBOARD(receiver.knightOrigins(), 0x4200000000000000);
  EXPECT_BITBOARD(receiver.knightTargets(), 0xA50000000000);
  EXPECT_EQ(receiver.rookMoveCount(), 0);
  EXPECT_EQ(receiver.queenMoveCount(), 0);
  EXPECT_EQ(receiver.kingMoveCount(), 0);
}

