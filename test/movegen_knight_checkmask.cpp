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

TEST(movegen_knight_checkmask, white_knight) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("7K/1k6/5n2/8/8/8/6B1/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_BITBOARD(receiver.knightOrigins(), 35184372088832ull, board);
  EXPECT_BITBOARD(receiver.knightTargets(), 34628173824ull, board);
}

TEST(movegen_knight_checkmask, black_knight) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("7K/1k6/5n2/8/8/8/6B1/8 w - - 0 1");
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 2);
  EXPECT_BITBOARD(receiver.knightOrigins(), 35184372088832ull, board);
  EXPECT_BITBOARD(receiver.knightTargets(), 34628173824ull, board);
}
