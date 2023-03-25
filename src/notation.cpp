//
// Created by karl on 25/03/23.
//
#include <sstream>

#include "notation.h"
#include "x86utils.h"

namespace notation {
std::string toString(const Board& board, const GameState& state,
                     uint64_t origin, uint64_t target, figure_type figure,
                     move_flag flag) {
  bitmap_t originTile = SQUARE_OF(origin);
  int originRank = originTile / 8;
  int originFile = originTile % 8;
  bitmap_t targetTile = SQUARE_OF(target);
  int targetRank = targetTile / 8;
  int targetFile = targetTile % 8;
  std::stringstream ss;

  switch (figure) {
    case PAWN:
      break;
    case BISHOP:
      ss << 'B';
      break;
    case KNIGHT:
      ss << 'N';
      break;
    case ROOK:
      ss << 'R';
      break;
    case QUEEN:
      ss << 'Q';
      break;
    case KING:
      ss << 'K';
      break;
    default:
      break;
  }

  if(flag == MOVE_FLAG_CAPTURE || flag == MOVE_FLAG_EN_PASSANT){
    ss << 'x';
  }

  ss << ((char) ('a' + targetFile));
  ss << ((char) ('1' + targetRank));

  return ss.str();
}
}  // namespace notation
