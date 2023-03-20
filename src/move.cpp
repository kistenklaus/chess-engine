//
// Created by karl on 20/03/23.
//
#include "move.h"

std::ostream& operator<<(std::ostream& cout, const runtime_move& move) {
  auto originSquare = tile_index(SQUARE_OF(move.m_origin));
  auto targetSquare = tile_index(SQUARE_OF(move.m_target));
  cout << move.m_figure << " : " << originSquare << " --> "
       << targetSquare;
  return cout;
}
