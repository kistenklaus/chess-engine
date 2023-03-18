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

TEST(movegen_rook_basic, white_rook_no_blocker_center) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board1 = fen::parse("7K/8/8/3R4/8/8/8/7k w - - 0 1");
  generate_moves<state>(board1, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 14);
  EXPECT_BITBOARD(receiver.rookOrigins(), 34359738368);
  EXPECT_BITBOARD(receiver.rookTargets(), 578722409201797128);
}

TEST(movegen_rook_basic, black_rook_no_blocker_center) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board1 =
      fen::parse("7K/8/8/3R4/8/8/8/7k w - - 0 1").invertBoard();
  generate_moves<state>(board1, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 14);
  EXPECT_BITBOARD(receiver.rookOrigins(), 34359738368);
  EXPECT_BITBOARD(receiver.rookTargets(), 578722409201797128);
}

TEST(movegen_rook_basic, white_rook_no_blocker_left_border) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7K/8/8/R7/8/8/8/7k w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 14);
  EXPECT_BITBOARD(receiver.rookOrigins(), 4294967296);
  EXPECT_BITBOARD(receiver.rookTargets(), 72341259464802561);
}

TEST(movegen_rook_basic, black_rook_no_blocker_left_border) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("7K/8/8/R7/8/8/8/7k w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 14);
  EXPECT_BITBOARD(receiver.rookOrigins(), 4294967296);
  EXPECT_BITBOARD(receiver.rookTargets(), 72341259464802561);
}

TEST(movegen_rook_basic, white_rook_no_blocker_top_border) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("3R4/8/8/8/8/8/8/K6k w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 14);
  EXPECT_BITBOARD(receiver.rookOrigins(), 576460752303423488ull);
  EXPECT_BITBOARD(receiver.rookTargets(), 17800486357769390088ull);
}

TEST(movegen_rook_basic, black_rook_no_blocker_top_border) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("3R4/8/8/8/8/8/8/K6k w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 14);
  EXPECT_BITBOARD(receiver.rookOrigins(), 576460752303423488ull);
  EXPECT_BITBOARD(receiver.rookTargets(), 17800486357769390088ull);
}

TEST(movegen_rook_basic, white_rook_no_blocker_right_border) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k7/8/8/7R/8/8/8/K7 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 14);
  EXPECT_BITBOARD(receiver.rookOrigins(), 549755813888);
  EXPECT_BITBOARD(receiver.rookTargets(), 9259542118978846848ull);
}

TEST(movegen_rook_basic, black_rook_no_blocker_right_border) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k7/8/8/7R/8/8/8/K7 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 14);
  EXPECT_BITBOARD(receiver.rookOrigins(), 549755813888);
  EXPECT_BITBOARD(receiver.rookTargets(), 9259542118978846848ull);
}

TEST(movegen_rook_basic, white_rook_no_blocker_bottom_border) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/8/8/8/8/8/8/3R4 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 14);
  EXPECT_BITBOARD(receiver.rookOrigins(), 8);
  EXPECT_BITBOARD(receiver.rookTargets(), 578721382704613623ull);
}

TEST(movegen_rook_basic, black_rook_no_blocker_bottom_border) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/8/8/8/8/8/8/3R4 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 14);
  EXPECT_BITBOARD(receiver.rookOrigins(), 8);
  EXPECT_BITBOARD(receiver.rookTargets(), 578721382704613623ull);
}

TEST(movegen_rook_basic, white_rook_friendly_blocker_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/8/8/8/2R2P2/8/8/8 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 11);
  EXPECT_BITBOARD(receiver.rookOrigins(), 67108864);
  EXPECT_BITBOARD(receiver.rookTargets(), 289360691738182660);
}

TEST(movegen_rook_basic, black_rook_friendly_blocker_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board =
      fen::parse("k6K/8/8/8/2R2P2/8/8/8 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 11);
  EXPECT_BITBOARD(receiver.rookOrigins(), 67108864);
  EXPECT_BITBOARD(receiver.rookTargets(), 289360691738182660);
}

TEST(movegen_rook_basic, white_rook_friendly_blocker_top) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/2P5/8/8/2R5/8/8/8 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 12);
  EXPECT_BITBOARD(receiver.rookOrigins(), 67108864);
  EXPECT_BITBOARD(receiver.rookTargets(), 4419437724676);
}

TEST(movegen_rook_basic, black_rook_friendly_blocker_top) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board =
      fen::parse("k6K/2P5/8/8/2R5/8/8/8 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 12);
  EXPECT_BITBOARD(receiver.rookOrigins(), 67108864);
  EXPECT_BITBOARD(receiver.rookTargets(), 4419437724676);
}

TEST(movegen_rook_basic, white_rook_friendly_blocker_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/8/8/2P2R2/8/8/8/8 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 11);
  EXPECT_BITBOARD(receiver.rookOrigins(), 137438953472);
  EXPECT_BITBOARD(receiver.rookTargets(), 2314886321092436000ull);
}

TEST(movegen_rook_basic, black_rook_friendly_blocker_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board =
      fen::parse("k6K/8/8/2P2R2/8/8/8/8 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 11);
  EXPECT_BITBOARD(receiver.rookOrigins(), 137438953472);
  EXPECT_BITBOARD(receiver.rookTargets(), 2314886321092436000ull);
}

TEST(movegen_rook_basic, white_rook_friendly_blocker_bottom) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board = fen::parse("k6K/8/8/5R2/8/8/5P2/8 w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 12);
  EXPECT_BITBOARD(receiver.rookOrigins(), 137438953472);
  EXPECT_BITBOARD(receiver.rookTargets(), 2314886351157198848ull);
}

TEST(movegen_rook_basic, black_rook_friendly_blocker_bottom) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;
  const Board board =
      fen::parse("k6K/8/8/5R2/8/8/5P2/8 w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.rookMoveCount(), 12);
  EXPECT_BITBOARD(receiver.rookOrigins(), 137438953472);
  EXPECT_BITBOARD(receiver.rookTargets(), 2314886351157198848ull);
}
