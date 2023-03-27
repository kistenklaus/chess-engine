//
// Created by karl on 27/03/23.
//
#pragma once
#include <vector>

#include "Board.h"
#include "ChessPosition.h"
#include "GameState.h"
#include "move.h"

namespace movegen::constant_collection {

class MoveCollector {
 public:
  explicit MoveCollector() : m_moves(nullptr) {}

  template <figure figure, move_flag flag>
  force_inline void move(bitmap_t origin, bitmap_t target) {
    *m_moves++ = runtime_move{.m_origin = origin,
                              .m_target = target,
                              .m_figure = figure,
                              .m_flag = flag};
  }

  force_inline void prepare(runtime_move* moves);

  [[nodiscard]] const runtime_move* getPtr() const;

 private:
  runtime_move* m_moves;
};

static MoveCollector g_collector;
static const unsigned int MAX_MOVES_PER_TURN = 218;
static runtime_move g_moveCache[MAX_MOVES_PER_TURN];

unsigned long runtime_entry_call(const Board& board, const GameState& state,
                                 bitmap_t epTarget, runtime_move* moves);

unsigned long runtime_entry_call(const ChessPosition& position,
                                 runtime_move* moves);

std::vector<runtime_move> runtime_entry_call(const ChessPosition& position);

}  // namespace movegen::constant_collection