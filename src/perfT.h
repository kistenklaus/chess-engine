//
// Created by karl on 25/03/23.
//

#include "Board.h"
#include "GameState.h"
#include "fen.h"

namespace perfT {

namespace internal {
unsigned long perfT(const Board& board, const GameState& state, int depth);
}

unsigned long run(const std::string& fen, int depth);

}  // namespace perfT
