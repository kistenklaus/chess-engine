//
// Created by karl on 23/03/23.
//

#include "Board.h"
#include "GameState.h"
#include "bitmap.h"
#include "move.h"
#include "move_generation.h"

template <GameState state, int depth>
class PerfTRecursion {
 public:
  explicit PerfTRecursion(const Board board) : m_board(board) {}

  template <figure_type figure, move_flag flag>
  void move(bitmap_t origin, bitmap_t target) {
    if constexpr (depth == 0) {
      m_count++;
    } else {
      compiletime_move<figure, flag> move = {.m_origin = origin,
                                             .m_target = target};

      Board nextBoard = m_board.applyMove<state>(move);
      constexpr GameState nextState =
          compiletimeStateTransition<state, figure, flag>();

      Movegen::PrepareEnumeration<state, figure, flag, depth>(m_board, move);
      auto recursion = PerfTRecursion<nextState, depth - 1>(nextBoard);
      m_count += recursion.run();
    }
  }

  unsigned long run() {
    Movegen::Enumerate<state, depth, PerfTRecursion<state, depth>>(m_board,
                                                                   *this);
    return m_count;
  }

  [[nodiscard]] unsigned long result() const { return m_count; }

 private:
  const Board m_board;
  unsigned long m_count = 0;
};