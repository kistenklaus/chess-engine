#pragma once

#include "Board.h"
#include "GameState.h"
#include "bitmap.h"
#include "fen.h"

class ChessPosition {
 private:
  Board* m_board = nullptr;
  GameState* m_state = nullptr;
  bitmap_t m_epTarget = 0;

 public:
  ChessPosition() = default;
  explicit ChessPosition(const Board& board, const GameState& state,
                         bitmap_t epTarget);
  ~ChessPosition();
  [[nodiscard]] const Board& board() const { return *m_board; }
  [[nodiscard]] const GameState& state() const { return *m_state; }
  [[nodiscard]] bitmap_t epTarget() const { return m_epTarget; }
  void update(const Board& board, const GameState& state, bitmap_t epTarget);

 public:
  static ChessPosition StartPosition() {
    return ChessPosition(fen::parse(fen::DEFAULT), GameState::Default(), 0);
  }
};
