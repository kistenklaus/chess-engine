//
// Created by karl on 17/03/23.
//

#include <gtest/gtest.h>

#include "Board.h"
#include "MoveTestReceiver.h"
#include "bitmap.h"
#include "fen.h"
#include "move_generation.h"

TEST(movegen_bishop_basic, white_bishop_no_blockers_center) {
  const Board board1 = fen::parse("7K/8/8/8/4B3/8/8/k7 w - - 0 1");

  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);

  MoveTestReceiver receiver;
  generate_moves<state>(board1, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 13);
  EXPECT_EQ(receiver.bishopOrigins(), 0x10000000);
  EXPECT_EQ(receiver.bishopTargets(), 0x182442800284482);
}

TEST(movegen_bishop_basic, black_bishop_no_blockers_center) {
  const Board board1 =
      fen::parse("7K/8/8/8/4B3/8/8/k7 w - - 0 1").invertBoard();

  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);

  MoveTestReceiver receiver;
  generate_moves<state>(board1, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 13);
  EXPECT_EQ(receiver.bishopOrigins(), 0x10000000);
  EXPECT_EQ(receiver.bishopTargets(), 0x182442800284482);
}

TEST(movegen_bishop_basic, white_bishop_no_blockers_top_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board2 = fen::parse("B6K/8/8/8/8/8/8/k7 w - - 0 1");
  generate_moves<state>(board2, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 7);
  EXPECT_EQ(receiver.bishopOrigins(), 72057594037927936);
  EXPECT_EQ(receiver.bishopTargets(), 567382630219904);
}

TEST(movegen_bishop_basic, black_bishop_no_blockers_top_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board2 = fen::parse("B6K/8/8/8/8/8/8/k7 w - - 0 1").invertBoard();
  generate_moves<state>(board2, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 7);
  EXPECT_EQ(receiver.bishopOrigins(), 72057594037927936);
  EXPECT_EQ(receiver.bishopTargets(), 567382630219904);
}

TEST(movegen_bishop_basic, white_bishop_no_blockers_bottom_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board3 = fen::parse("7K/8/8/8/8/8/8/k6B w - - 0 1");
  generate_moves<state>(board3, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 7);
  EXPECT_EQ(receiver.bishopOrigins(), 128);
  EXPECT_EQ(receiver.bishopTargets(), 72624976668147712);
}

TEST(movegen_bishop_basic, black_bishop_no_blockers_bottom_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board3 = fen::parse("7K/8/8/8/8/8/8/k6B w - - 0 1").invertBoard();
  generate_moves<state>(board3, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 7);
  EXPECT_EQ(receiver.bishopOrigins(), 128);
  EXPECT_EQ(receiver.bishopTargets(), 72624976668147712);
}

TEST(movegen_bishop_basic, white_bishop_friendly_blocker_up_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  const Board board1 = fen::parse("7K/8/8/3P4/8/8/8/k6B w - - 0 1");
  MoveTestReceiver receiver;
  generate_moves<state>(board1, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 3);
  EXPECT_EQ(receiver.bishopOrigins(), 128);
  EXPECT_EQ(receiver.bishopTargets(), 270548992);
}

TEST(movegen_bishop_basic, black_bishop_friendly_blocker_up_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  const Board board1 =
      fen::parse("7K/8/8/3P4/8/8/8/k6B w - - 0 1").invertBoard();
  MoveTestReceiver receiver;
  generate_moves<state>(board1, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 3);
  EXPECT_EQ(receiver.bishopOrigins(), 128);
  EXPECT_EQ(receiver.bishopTargets(), 270548992);
}

TEST(movegen_bishop_basic, white_bishop_friendly_blocker_up_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board2 = fen::parse("k7/8/8/4P3/8/8/8/B6K w - - 0 1");
  generate_moves<state>(board2, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 3);
  EXPECT_EQ(receiver.bishopOrigins(), 1);
  EXPECT_EQ(receiver.bishopTargets(), 134480384);
}

TEST(movegen_bishop_basic, black_bishop_friendly_blocker_up_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board2 =
      fen::parse("k7/8/8/4P3/8/8/8/B6K w - - 0 1").invertBoard();
  generate_moves<state>(board2, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 3);
  EXPECT_EQ(receiver.bishopOrigins(), 1);
  EXPECT_EQ(receiver.bishopTargets(), 134480384);
}

TEST(movegen_bishop_basic, white_bishop_friendly_blocker_down_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board3 = fen::parse("B7/8/8/8/4P3/8/3k4/7K w - - 0 1");
  generate_moves<state>(board3, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 3);
  EXPECT_EQ(receiver.bishopOrigins(), 72057594037927936);
  EXPECT_EQ(receiver.bishopTargets(), 567382359670784);
}

TEST(movegen_bishop_basic, black_bishop_friendly_blocker_down_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board3 =
      fen::parse("B7/8/8/8/4P3/8/3k4/7K w - - 0 1").invertBoard();
  generate_moves<state>(board3, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 3);
  EXPECT_EQ(receiver.bishopOrigins(), 72057594037927936);
  EXPECT_EQ(receiver.bishopTargets(), 567382359670784);
}

TEST(movegen_bishop_basic, white_bishop_friendly_blocker_down_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board4 = fen::parse("7B/8/8/8/3P4/8/3k4/7K w - - 0 1");
  generate_moves<state>(board4, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 3);
  EXPECT_EQ(receiver.bishopOrigins(), 9223372036854775808ull);
  EXPECT_EQ(receiver.bishopTargets(), 18049651601047552ull);
}

TEST(movegen_bishop_basic, black_bishop_friendly_blocker_down_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board4 =
      fen::parse("7B/8/8/8/3P4/8/3k4/7K w - - 0 1").invertBoard();
  generate_moves<state>(board4, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 3);
  EXPECT_EQ(receiver.bishopOrigins(), 9223372036854775808ull);
  EXPECT_EQ(receiver.bishopTargets(), 18049651601047552ull);
}

TEST(movegen_bishop_basic, white_bishop_enemy_blocker_up_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board = fen::parse("7K/8/8/3p4/8/8/8/k6B w - - 0 1");
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 4);
  EXPECT_EQ(receiver.bishopOrigins(), 128);
  EXPECT_EQ(receiver.bishopTargets(), 34630287360);
}

TEST(movegen_bishop_basic, black_bishop_enemy_blocker_up_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board =
      fen::parse("7K/8/8/3p4/8/8/8/k6B w - - 0 1").invertBoard();
  generate_moves<state>(board, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 4);
  EXPECT_EQ(receiver.bishopOrigins(), 128);
  EXPECT_EQ(receiver.bishopTargets(), 34630287360);
}

TEST(movegen_bishop_basic, white_bishop_enemy_blocker_up_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board2 = fen::parse("k7/8/8/4p3/8/8/8/B6K w - - 0 1");
  generate_moves<state>(board2, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 4);
  EXPECT_EQ(receiver.bishopOrigins(), 1);
  EXPECT_EQ(receiver.bishopTargets(), 68853957120);
}

TEST(movegen_bishop_basic, black_bishop_enemy_blocker_up_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board2 =
      fen::parse("k7/8/8/4p3/8/8/8/B6K w - - 0 1").invertBoard();
  generate_moves<state>(board2, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 4);
  EXPECT_EQ(receiver.bishopOrigins(), 1);
  EXPECT_EQ(receiver.bishopTargets(), 68853957120);
}

TEST(movegen_bishop_basic, white_bishop_enemy_blocker_down_right) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board3 = fen::parse("B7/8/8/8/4p3/8/3k4/7K w - - 0 1");
  generate_moves<state>(board3, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 4);
  EXPECT_EQ(receiver.bishopOrigins(), 72057594037927936);
  EXPECT_EQ(receiver.bishopTargets(), 567382628106240);
}

TEST(movegen_bishop_basic, black_bishop_enemy_blocker_down_right) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board3 = fen::parse("B7/8/8/8/4p3/8/3k4/7K w - - 0 1")
                           .invertBoard();
  generate_moves<state>(board3, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 4);
  EXPECT_EQ(receiver.bishopOrigins(), 72057594037927936);
  EXPECT_EQ(receiver.bishopTargets(), 567382628106240);
}

TEST(movegen_bishop_basic, white_bishop_enemy_blocker_down_left) {
  constexpr BoardState state =
      BoardState(WHITE, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board4 = fen::parse("7B/8/8/8/3p4/8/3k4/7K w - - 0 1");
  generate_moves<state>(board4, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 4);
  EXPECT_EQ(receiver.bishopOrigins(), 9223372036854775808ull);
  EXPECT_EQ(receiver.bishopTargets(), 18049651735265280);
}

TEST(movegen_bishop_basic, black_bishop_enemy_blocker_down_left) {
  constexpr BoardState state =
      BoardState(BLACK, false, false, false, false, false);
  MoveTestReceiver receiver;

  const Board board4 = fen::parse("7B/8/8/8/3p4/8/3k4/7K w - - 0 1")
                           .invertBoard();
  generate_moves<state>(board4, receiver);
  EXPECT_EQ(receiver.bishopMoveCount(), 4);
  EXPECT_EQ(receiver.bishopOrigins(), 9223372036854775808ull);
  EXPECT_EQ(receiver.bishopTargets(), 18049651735265280);
}
