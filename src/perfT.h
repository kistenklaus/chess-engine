//
// Created by karl on 25/03/23.
//

#include "Board.h"
#include "GameState.h"
#include "fen.h"
#include "ChessPosition.h"

namespace perfT {

namespace internal {
unsigned long perfT(const Board& board, const GameState& state, int depth);
}

unsigned long run(const ChessPosition& position, int depth);

}  // namespace perfT
