#include "ChessPosition.h"

ChessPosition::ChessPosition(const Board& board, const GameState& state,
                             bitmap_t epTarget)
    : m_board(new Board(board)),
      m_state(new GameState(state)),
      m_epTarget(epTarget) {}

ChessPosition::~ChessPosition() {
  delete m_board;
  delete m_state;
}

void ChessPosition::update(const Board& board, const GameState& state,
                           bitmap_t epTarget) {
  m_board = new Board(board);
  m_state = new GameState(state);
  m_epTarget = epTarget;
}
