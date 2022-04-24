
#pragma once
#include "Board.h"
#include "BoardState.h"
#include "move.h"

template <class BoardState state>
inline void generate_moves(const Board& board,
                           void (*move_callback)(const move_t)) {
  if constexpr (state.has_long_castle()) {
  }
  constexpr auto s = state.register_king_move();
  generate_moves<s>(board, move_callback);
}
