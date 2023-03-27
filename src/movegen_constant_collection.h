//
// Created by karl on 26/03/23.
//
#pragma once
#include "movegen_constant_enumeration.h"

namespace movegen::constant_collection {


template <GameState state, figure figure, move_flag flag, int depth>
force_inline void prepare(const Board& board,
                          const compiletime_move<figure, flag>& move) {
  constant_enumeration::prepare<state, figure, flag, depth>(board, move);
}

template <GameState state, int depth>
unsigned long recursive_call(const Board& board, runtime_move* moves) {
  g_collector.prepare(moves);
  constant_enumeration::recursive_call<state, depth, MoveCollector>(
      board, g_collector);
  return g_collector.getPtr() - moves;
}

template <GameState state, int depth>
unsigned long entry_call(const Board& board, bitmap_t epTarget,
                         runtime_move* moves) {
  g_collector.prepare(moves);
  constant_enumeration::entry_call<state, depth, MoveCollector>(board, epTarget,
                                                                g_collector);
  return g_collector.getPtr() - moves;
}


}  // namespace movegen::constant_collection
