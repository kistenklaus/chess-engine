//
// Created by karl on 27/03/23.
//
#include "movegen_constant_collection_decl.h"
#include "movegen_constant_collection.h"

namespace movegen::constant_collection {

void MoveCollector::prepare(runtime_move* moves) { m_moves = moves; }

const runtime_move* MoveCollector::getPtr() const { return m_moves; }

unsigned long runtime_entry_call(const Board& board, const GameState& state,
                                 uint64_t epTarget, runtime_move* moves) {
  g_collector.prepare(moves);
  constant_enumeration::runtime_entry_call<MoveCollector>(
      board, state, epTarget, g_collector);
  return g_collector.getPtr() - moves;
}

unsigned long runtime_entry_call(const ChessPosition& position,
                                 runtime_move* moves) {
  g_collector.prepare(moves);
  constant_enumeration::runtime_entry_call<MoveCollector>(
      position.board(), position.state(), position.epTarget(), g_collector);
  return g_collector.getPtr() - moves;
}

std::vector<runtime_move> runtime_entry_call(const ChessPosition& position) {
  const unsigned long count = runtime_entry_call(position, &(g_moveCache[0]));
  std::vector<runtime_move> moveVector(g_moveCache, g_moveCache + count);
  return moveVector;
}
}