//
// Created by karl on 21/03/23.
//
#include <string>

#include "Board.h"
#include "move.h"

runtime_move parseAlgebraicNotation(const Board& board, const GameState& state,
                                    const std::string& algNotation);

std::string toAlgebraicNotation(const Board& board, const GameState& state,
                                const runtime_move& move);
