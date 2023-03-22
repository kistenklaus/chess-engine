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


TEST(movegen_knight_pinmask, white_knight) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/1k6/6K1/8/4N3/8/2b5/8 w - - 0 1");
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.knightMoveCount(), 0);
  EXPECT_BITBOARD(receiver.knightOrigins(), 0, board);
  EXPECT_BITBOARD(receiver.knightTargets(), 0, board);
}

