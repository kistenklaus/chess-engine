#pragma once

#include "Board.h"
#include "bitmap.h"

namespace fen {
Board parse(const std::string FEN);

} // namespace fen
