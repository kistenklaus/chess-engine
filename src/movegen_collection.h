//
// Created by karl on 26/03/23.
//
#pragma once
#include "movegen_const_collection.h"
#include "movegen_enumeration.h"

namespace movegen {

template <int depth>
void collect(const Board& board, const GameState& state,
             std::vector<runtime_move>& moves) {
  collection::MoveVectorCollector collector(moves);
  enumerate<depth>(board, state, collector);
}

void collect(const Board& board, const GameState& state,
             std::vector<runtime_move>& moves) {
  collection::MoveVectorCollector collector(moves);
  enumerate(board, state, collector);
}

std::vector<runtime_move> collect(const Board& board, const GameState& state) {
  std::vector<runtime_move> moves;
  collection::MoveVectorCollector collector(moves);
  enumerate(board, state, collector);
  return moves;
}

}  // namespace movegen
