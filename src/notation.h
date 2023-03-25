//
// Created by karl on 25/03/23.
//
#pragma once
#include <string>

#include "Board.h"
#include "figure.h"
#include "move.h"

namespace notation {
std::string toString(const Board& board, const GameState& state,
                     bitmap_t origin, bitmap_t target, figure_type figure,
                     move_flag flag);
}
