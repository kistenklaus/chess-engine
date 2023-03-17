#pragma once

#include "color.h"

class BoardCompiletimeState {
 private:
  color_t m_turn;

 public:
  explicit BoardCompiletimeState(color_t turn) : m_turn(turn) {}

  [[nodiscard]] inline constexpr color_t turn() const { return m_turn; }

  [[nodiscard]] inline constexpr bool isWhitesTurn() const {
    return m_turn == WHITE;
  }

  [[nodiscard]] inline constexpr bool isBlackTurn() const {
    return m_turn == BLACK;
  }

  [[nodiscard]] inline constexpr BoardCompiletimeState nextTurn() const {
    return BoardCompiletimeState(!m_turn);
  }

};
