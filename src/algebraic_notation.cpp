//
// Created by karl on 21/03/23.
//
#include "algebraic_notation.h"

#include <algorithm>
#include <ranges>
#include <sstream>
#include <utility>

#include "RuntimeMoveCollector.h"
#include "move_generation.h"

static void error() {
  throw std::invalid_argument("failed to parse algebraic notation");
}

struct lexer {
 private:
  const std::string& m_notation;
  unsigned int m_index;

 public:
  lexer(const std::string& notation, unsigned int index)
      : m_notation(notation), m_index(index) {}

  void nextToken() { m_index++; }

  bool eoi() { return m_index >= m_notation.size(); }

  void expect(char c) {
    if (getToken() != c) error();
    nextToken();
  }

  char getToken() { return m_notation[m_index]; }

  bool isFile() {
    return m_notation[m_index] >= 'a' && m_notation[m_index] <= 'h';
  }

  bool isRank() {
    return m_notation[m_index] >= '0' && m_notation[m_index] <= '8';
  }

  bool isPiece() {
    const char c = m_notation[m_index];
    switch (c) {
      case 'N':
      case 'B':
      case 'R':
      case 'Q':
      case 'K':
        return true;
      default:
        return false;
    }
  }
};

static unsigned int parseFile(lexer& lex) {
  unsigned int file = lex.getToken() - 'a';
  lex.nextToken();
  return file;
}

static unsigned int parseRank(lexer& lex) {
  unsigned int rank = lex.getToken() - '1';
  lex.nextToken();
  return rank;
}

static void parsePiece(lexer& lex, runtime_move* move) {
  switch (lex.getToken()) {
    case 'N':
      move->m_figure = KNIGHT;
      break;
    case 'B':
      move->m_figure = BISHOP;
      break;
    case 'R':
      move->m_figure = ROOK;
      break;
    case 'Q':
      move->m_figure = QUEEN;
      break;
    case 'K':
      move->m_figure = KING;
      break;
    default:
      error();
  }
  lex.nextToken();
}

static void parseT(lexer& lex, runtime_move* move) {
  unsigned int file = parseRank(lex);
  unsigned int rank = parseFile(lex);
  move->m_target = ((bitmap_t)1) << (rank*8+file);
}

static void parseExtra(lexer& lex, runtime_move* move) {
  if (lex.eoi()) {
    return;
  } else if (lex.getToken() == '+') {
    move->m_flags |= MOVE_FLAG_CHECK;
  } else if (lex.getToken() == '#') {
    move->m_flags |= MOVE_FLAG_CHECK;  // TODO add checkmate flag.
  } else {
    error();
  }
}

static void parseB(lexer& lex, runtime_move* move) {
  if (lex.getToken() == 'x') {
    lex.expect('x');
    move->m_flags |= MOVE_FLAG_CAPTURE;
    parseT(lex, move);
    parseExtra(lex, move);
  } else if (lex.isFile()) {
    parseT(lex, move);
    parseExtra(lex, move);
  } else if (lex.isRank()) {
    unsigned int rank = parseRank(lex);
    move->m_target &= ((bitmap_t)0xFF) << (rank * 8);
    parseExtra(lex, move);
  }
}

static void parseA(lexer& lex, runtime_move* move) {
  if (lex.getToken() == 'x') {
    lex.expect('x');
    move->m_flags |= MOVE_FLAG_CAPTURE;
    parseT(lex, move);
    parseExtra(lex, move);
  } else if (lex.isFile()) {
    parseT(lex, move);
    parseExtra(lex, move);
  };
}

static void parseDescription(lexer& lex, runtime_move* move) {
  if (lex.getToken() == 'x') {
    lex.expect('x');
    move->m_flags |= MOVE_FLAG_CAPTURE;
    parseT(lex, move);
    parseExtra(lex, move);
  } else if (lex.isRank()) {
    unsigned int rank = parseRank(lex);
    parseA(lex, move);
    const bitmap_t mask = ((bitmap_t)0xFF) << (rank * 8);
    if (REST_LOWEST_BIT(move->m_target) == 0) {
      move->m_origin &= mask;
    } else {
      move->m_target &= mask;
    }
  } else if (lex.isFile()) {
    unsigned int file = parseFile(lex);
    parseB(lex, move);
    bitmap_t mask = ((bitmap_t)0x101010101010101) << file;
    if (REST_LOWEST_BIT(move->m_target) == 0) {
      move->m_origin &= mask;
    } else {
      move->m_target &= mask;
    }
  }
}

runtime_move parseAlgebraicNotation(const Board& board, const GameState& state,
                                    const std::string& algNotation) {
  runtime_move parsed_move{.m_origin = 0xFFFFFFFFFFFFFFFF,
                           .m_target = 0xFFFFFFFFFFFFFFFF,
                           .m_figure = NOT_A_FIGURE,
                           .m_flags = 0};
  lexer lex(algNotation, 0);
  if (lex.isPiece()) {
    parsePiece(lex, &parsed_move);
  } else {
    parsed_move.m_figure = PAWN;
  }
  parseDescription(lex, &parsed_move);
  // finalize description of origin and validate flags.
  RuntimeMoveCollector collector;
  generate_moves(board, state, collector);

  for (const runtime_move& move : collector.moves()) {
    if (move.m_target == parsed_move.m_target &&
        move.m_figure == parsed_move.m_figure &&
        REST_LOWEST_BIT(move.m_origin & parsed_move.m_origin) == 0) {
      return move;
    }
  }
  error();
}

static char rankToChar(unsigned int rank) {
  if (rank > 8) throw std::runtime_error("FUCK");
  return static_cast<char>('0') + (char)(rank + 1);
}

static char fileToChar(unsigned int file) {
  if (file > 8) throw std::runtime_error("FUCK");
  return static_cast<char>('a') + (char)file;
}

std::string toAlgebraicNotation(const Board& board, const GameState& state,
                                const runtime_move& move) {
  RuntimeMoveCollector collector;
  generate_moves(board, state, collector);
  std::vector<runtime_move> similarMoves;
  for (const runtime_move& allowedMove : collector.moves()) {
    if (allowedMove.m_target == move.m_target &&
        allowedMove.m_figure == move.m_figure) {
      similarMoves.push_back(allowedMove);
    }
  }
  if (similarMoves.empty()) {
    throw std::runtime_error(
        "failed to convert move into algebraic notation "
        "[not a valid move]");
  }
  std::stringstream ss;
  switch (move.m_figure) {
    case BISHOP:
      ss << "B";
      break;
    case KNIGHT:
      ss << "N";
      break;
    case ROOK:
      ss << "R";
      break;
    case QUEEN:
      ss << "Q";
      break;
    case KING:
      ss << "K";
      break;
    case PAWN:
      break;
    default:
      throw std::runtime_error("failed to convert move to algebraic notation");
  }
  unsigned int targetTile = SQUARE_OF(move.m_target);
  unsigned int targetRank = targetTile / 8;
  unsigned int targetFile = targetTile % 8;
  if (similarMoves.size() > 1) {
    // add origin hint to notation.
    int originRank = -1;
    for (const runtime_move& m : similarMoves) {
      if (originRank == -1) {
        originRank = SQUARE_OF(m.m_origin) / 8;
      } else if (originRank != SQUARE_OF(m.m_origin) / 8) {
        originRank = -1;
        break;
      }
    }
    if (originRank != -1) {
      unsigned int originFile = SQUARE_OF(move.m_origin) % 8;
      ss << fileToChar(originFile);
    } else {
      unsigned int originRank2 = SQUARE_OF(move.m_origin) / 8;
      ss << rankToChar(originRank2);
      // TODO: In Theory a move like Ba1b2 would be possible which is not
      //  handled.
    }
  }

  if (move.m_flags & MOVE_FLAG_CAPTURE) {
    ss << "x";
  }
  ss << fileToChar(targetFile);
  ss << rankToChar(targetRank);
  if (move.m_flags & MOVE_FLAG_CHECK) {
    ss << '+';
  }
  return ss.str();
}
