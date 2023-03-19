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


TEST(movegen_bishop_pinmask, white_bishop_pinned_axial) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/8/k7/8/2r1B1K1/8/8/8 w - - 0 1");
  MoveTestReceiver receiver;
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 0);
  EXPECT_BITBOARD(receiver.bishopOrigins(), 0, board);
  EXPECT_BITBOARD(receiver.bishopTargets(), 0, board);
}
