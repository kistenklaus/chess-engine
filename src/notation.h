//
// Created by karl on 25/03/23.
//
#pragma once
#include <string>

#include "Board.h"
#include "ChessPosition.h"
#include "figure.h"
#include "move.h"

namespace notation {

runtime_move parse(const ChessPosition& position, const std::string& notation);

std::string toString(const ChessPosition& position, bitmap_t origin,
                     bitmap_t target, figure figure, move_flag flag);

std::string toString(const Board& board, const GameState& state,
                     bitmap_t epTarget, bitmap_t origin, bitmap_t target,
                     figure figure, move_flag flag);

std::string toString(const ChessPosition& position, const runtime_move& move);

std::string toString(bitmap_t tile);

template <figure figure, move_flag flag>
std::string toString(const ChessPosition& position,
                     const compiletime_move<figure, flag>& move) {
  return toString(position, move.m_origin, move.m_target, figure, flag);
}

}  // namespace notation
