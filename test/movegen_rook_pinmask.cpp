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


TEST(movegen_rook_pinmask, white_rook_pinned_diagonally) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1k6/6K1/8/4R3/8/2b5/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 0);
  EXPECT_BITBOARD(receiver.rookOrigins(), 0, board);
  EXPECT_BITBOARD(receiver.rookTargets(), 0, board);
}

TEST(movegen_rook_pinmask, black_rook_pinned_diagonally) {
  constexpr GameState state =
      GameState(BLACK, false, false, false, false, false);
  const Board board = fen::parse("8/1k6/6K1/8/4R3/8/2b5/8 w - - 0 1")
                          .invertBoard();
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 0);
  EXPECT_BITBOARD(receiver.rookOrigins(), 0, board);
  EXPECT_BITBOARD(receiver.rookTargets(), 0, board);
}
