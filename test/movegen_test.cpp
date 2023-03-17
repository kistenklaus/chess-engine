
#include <gtest/gtest.h>
#include <sys/types.h>

#include "Board.h"
#include "bitmap.h"
#include "fen.h"
#include "move_generation.h"
#include "x86utils.h"

template <class BoardState state>
void test_util(std::string board_fen, int expected_move_count,
               bitmap_t expected_targets, bitmap_t expected_origins) {
  test_util<state>(board_fen, expected_move_count, expected_targets,
                   expected_origins, figure_type::EMPTY);
}

template <class BoardState state>
void test_util(std::string board_fen, int expected_move_count,
               bitmap_t expected_targets, bitmap_t expected_origins,
               figure_type figure) {
  Board board = fen::parse(board_fen);
  int move_count = 0;
  bitmap_t targets = 0;
  bitmap_t origins = 0;

  std::cout.setstate(std::ios_base::failbit);
  generate_moves<state>(board, [&](const move_t move) {
    if (!figure ||
        move.prev_board.get_figure_at(SQUARE_OF(move.origin)).type() ==
            figure) {
      move_count++;
      targets |= move.target;
      origins |= move.origin;
    }
  });
  std::cout.clear();

  EXPECT_EQ(expected_move_count, move_count);
  EXPECT_EQ(expected_targets, targets) << bitmap_to_bitboard_string(targets);
  EXPECT_EQ(expected_origins, origins) << bitmap_to_bitboard_string(origins);
  if (expected_move_count != move_count || expected_targets != targets ||
      expected_origins != origins) {
    std::cout << board << std::endl;
  }
}

TEST(MoveGeneration, starting_position_movegeneration) {
  EXPECT_EQ(true, true);
  test_util<BoardState::Default()>(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 20,
      4294901760, 65346, figure_type::EMPTY);
}

TEST(MoveGeneration, queen_no_blocker_movegeneration) {
  test_util<BoardState(WHITE, false, false, false, false, false)>(
      "2k5/8/K7/8/4Q3/8/8/8 w - - 0 1", 0, 0, 0, figure_type::QUEEN);
}
