#pragma once

#include "Board.h"
#include "bitmap.h"

namespace fen {
Board parse(const std::string& FEN);
const std::string DEFAULT =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w "
    "KQkq - 0 1";

}  // namespace fen
