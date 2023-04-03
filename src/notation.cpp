//
// Created by karl on 25/03/23.
//
#include "notation.h"

#include <sstream>
#include <vector>

#include "ChessPosition.h"
#include "movegen_constant_collection_decl.h"
#include "x86utils.h"

namespace notation {

static bool isRank(char c) { return c >= '1' && c <= '8'; }
static bool isFile(char c) { return c >= 'a' && c <= 'h'; }

static bool isFigure(char c) {
  return (c == 'B') || (c == 'N') || (c == 'R') || (c == 'Q') || (c == 'K');
}

static int parseRank(char c) { return c - '1'; }

static int parseFile(char c) { return c - 'a'; }

static figure parseFigure(char c) {
  switch (c) {
    case 'B':
      return BISHOP;
    case 'N':
      return KNIGHT;
    case 'R':
      return ROOK;
    case 'Q':
      return QUEEN;
    case 'K':
      return KING;
    default:
      throw std::invalid_argument(
          "failed to parse the figure from a "
          "char");
  }
}

runtime_move parse(const ChessPosition& position, const std::string& notation) {
  if (notation.empty())
    throw std::invalid_argument(
        "can't parse empty notation"
        " string");
  bool capture = false;

  runtime_move move{.m_origin = 0xFFFFFFFFFFFFFFFF};
  // ignore + and # flags
  int e = notation.length() - 1;
  if (notation[e] == '#' || notation[e] == '+') {
    e -= 1;
  }
  if (!isRank(notation[e]) || !isFile(notation[e - 1])) {
    throw std::invalid_argument(
        "can't parse chess move, because the target "
        "square can't be determined");
  } else {
    int targetRank = parseRank(notation[e]);
    int targetFile = parseFile(notation[e - 1]);
    move.m_target = 1ull << (targetRank * 8 + targetFile);
    e -= 2;
  }
  if (e >= 0 && notation[e] == 'x') {
    capture = true;
    e -= 1;
  }
  if (0 > e) {
    move.m_figure = PAWN;
  } else if (0 >= e && isFigure(notation[e])) {
    move.m_figure = parseFigure(notation[e]);
    e -= 1;
  } else {
    throw std::invalid_argument("failed to parse figure type of notated move");
  }
  if (1 >= e && isRank(notation[e]) && isFile(notation[e - 1])) {
    int originRank = parseRank(notation[e]);
    int originFile = parseFile(notation[e - 1]);
    move.m_origin = 1ull << (originRank * 8 + originFile);
  } else if (0 >= e && isRank(notation[e]) || isFile(notation[e])) {
    if (isRank(notation[e])) {
      int originRank = parseRank(notation[e]);
      move.m_origin = 0xFFull << originRank;
    } else if (isFile(notation[e])) {
      int originFile = parseFile(notation[e]);
      move.m_origin = 0x101010101010101ull << originFile;
    }
  }
  // TODO match move to actually possible moves.
  std::vector<runtime_move> allowedMoves =
      movegen::constant_collection::runtime_entry_call(position);

  std::vector<runtime_move> matchingMoves;
  for (const runtime_move& allowedMove : allowedMoves) {
    if (allowedMove.m_target == move.m_target &&
        allowedMove.m_figure == move.m_figure &&
        (allowedMove.m_origin & move.m_origin) != 0) {
      matchingMoves.push_back(allowedMove);
    }
  }
  if(matchingMoves.size() > 1){
    throw std::invalid_argument("move is ambiguous");
  }else if(matchingMoves.empty()){
    throw std::invalid_argument("move not allowed");
  }else{
    return matchingMoves[0];
  }
}

std::string toString(const Board& board, const GameState& state,
                     bitmap_t epTarget, uint64_t origin, uint64_t target,
                     figure figure, move_flag flag) {
  bitmap_t originTile = SQUARE_OF(origin);
  unsigned int originRank = originTile / 8;
  unsigned int originFile = originTile % 8;
  bitmap_t targetTile = SQUARE_OF(target);
  unsigned int targetRank = targetTile / 8;
  unsigned int targetFile = targetTile % 8;
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

  if (flag == MOVE_FLAG_CAPTURE || flag == MOVE_FLAG_EN_PASSANT) {
    ss << 'x';
  }

  ss << ((char)('a' + targetFile));
  ss << ((char)('1' + targetRank));

  return ss.str();
}

std::string toString(const ChessPosition& position, uint64_t origin,
                     uint64_t target, figure figure, move_flag flag) {
  return toString(position.board(), position.state(), position.epTarget(),
                  origin, target, figure, flag);
}

std::string toString(const ChessPosition& position, const runtime_move& move) {
  return toString(position, move.m_origin, move.m_target, move.m_figure,
                  move.m_flag);
}
std::string toString(bitmap_t map) {
  bitmap_t tile = SQUARE_OF(map);
  unsigned int rank = tile / 8;
  unsigned int file = tile % 8;
  std::stringstream ss;
  ss << ((char) ('a' + file));
  ss << ((char) ('1' + rank));
  return ss.str();
}

}  // namespace notation
