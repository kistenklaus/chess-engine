//
// Created by karl on 23/03/23.
//

#include "Board.h"
#include "GameState.h"
#include "bitmap.h"
#include "move.h"
#include "move_generation.h"

template <GameState state>
class PerfTRecursion {
 public:
  explicit PerfTRecursion(Board board, unsigned int depth)
      : m_board(board), m_depth(depth), m_checkmask(generate_checkmask<state>
          (board)), m_pinmask(generate_pinmask<state>(board)),
              m_banmask(generate_banmask<state>(board))
  {

  }

  template <figure_type figure, compiletime_move_flag cflag>
  void move(bitmap_t origin, bitmap_t target, move_flag rflag) {
    if (m_depth == 0) {
      m_count++;
    } else {
      compiletime_move<figure, cflag> move = {
          .m_origin = origin, .m_target = target, .m_flags = rflag};
      Board nextBoard = m_board.applyMove<state>(move);
      constexpr GameState nextState =
          compiletimeStateTransition<state, cflag>();
      auto recursion = PerfTRecursion<nextState>(nextBoard, m_depth - 1);
      recursion.start();
      m_count += recursion.result();
    }
  }

  void start() {
    enumerateMoves<state>(m_board, *this, m_checkmask, m_pinmask, m_banmask);
  }

  [[nodiscard]] unsigned long result() const { return m_count; }

 private:
  const Board m_board;
  unsigned int m_depth;
  const checkmask_t m_checkmask;
  const pinmask_t m_pinmask;
  const banmask_t m_banmask;
  unsigned long m_count = 0;
};