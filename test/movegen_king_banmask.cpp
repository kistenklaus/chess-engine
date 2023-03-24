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

TEST(movegen_king_banmask, white_king) {
  constexpr GameState state =
      GameState(WHITE, false, false, false, false, false);
  const Board board = fen::parse("8/3k4/5b2/8/8/3K4/8/8 w - - 0 1");
  MoveTestReceiver receiver;
  enumerateMoves<state>(board, receiver);
  EXPECT_EQ(receiver.kingMoveCount(), 6);
  EXPECT_BITBOARD(receiver.kingOrigins(), 524288, board);
  EXPECT_BITBOARD(receiver.kingTargets(), 336600064, board);
}