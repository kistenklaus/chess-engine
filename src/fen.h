#pragma once

#include "Board.h"
#include "bitmap.h"
#include "ChessPosition.h"


namespace fen {

ChessPosition parsePosition(const std::string& FEN);

Board parse(const std::string& FEN);

const std::string DEFAULT =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w "
    "KQkq - 0 1";

}  // namespace fen
