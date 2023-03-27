#pragma once

#include "Board.h"
#include "GameState.h"
#include "bitmap.h"
#include "fen.h"

class ChessPosition {
 private:
  Board m_board;
  GameState m_state;
  bitmap_t m_epTarget;
 public:

  explicit ChessPosition()
      : m_board(fen::parse(fen::DEFAULT)),
        m_state(GameState::Default()),
        m_epTarget(0) {}

  explicit ChessPosition(const Board& board, const GameState& state,
                         bitmap_t epTarget)
      : m_board(board), m_state(state), m_epTarget(epTarget) {}

  ChessPosition(const ChessPosition& other) = default;

  ChessPosition(ChessPosition&& other) = default;

  ChessPosition& operator=(const ChessPosition& other) { m_board = other.m_board;
    m_state = other.m_state;
    m_epTarget = other.m_epTarget;
    return *this;
  }

  ChessPosition& operator=(ChessPosition&& other) noexcept {
    m_board = other.m_board;
    m_state = other.m_state;
    m_epTarget = other.m_epTarget;
    return *this;
  }

  [[nodiscard]] const Board& board() const { return m_board; }
  [[nodiscard]] const GameState& state() const { return m_state; }
  [[nodiscard]] bitmap_t epTarget() const { return m_epTarget; }

  bool getSquareOccupation(int tile, figure* figure, color_t* color) const {
    bitmap_t mask = 1ull << tile;
    if ((m_board.Occupied() & mask) == 0) return false;
    if (m_board.Pawns<WHITE>() & mask) {
      *color = WHITE;
      *figure = PAWN;
    } else if (m_board.Pawns<BLACK>() & mask) {
      *color = BLACK;
      *figure = PAWN;
    } else if (m_board.Bishops<WHITE>() & mask) {
      *color = WHITE;
      *figure = BISHOP;
    } else if (m_board.Bishops<BLACK>() & mask) {
      *color = BLACK;
      *figure = BISHOP;
    } else if (m_board.Knights<WHITE>() & mask) {
      *color = WHITE;
      *figure = KNIGHT;
    } else if (m_board.Knights<BLACK>() & mask) {
      *color = BLACK;
      *figure = KNIGHT;
    } else if (m_board.Rooks<WHITE>() & mask) {
      *color = WHITE;
      *figure = ROOK;
    } else if (m_board.Rooks<BLACK>() & mask) {
      *color = BLACK;
      *figure = ROOK;
    } else if (m_board.Queens<WHITE>() & mask) {
      *color = WHITE;
      *figure = QUEEN;
    } else if (m_board.Queens<BLACK>() & mask) {
      *color = BLACK;
      *figure = QUEEN;
    } else if (m_board.King<WHITE>() & mask) {
      *color = WHITE;
      *figure = KING;
    } else if (m_board.King<BLACK>() & mask) {
      *color = BLACK;
      *figure = KING;
    } else {
      throw std::runtime_error("wtf is going on with this board");
    }
    return true;
  }

 public:
  static ChessPosition StartPosition() {
    return ChessPosition();
  }
};
