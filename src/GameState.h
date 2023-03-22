#pragma once
#include "color.h"
#include "figure.h"
#include "move.h"

class GameState {
 public:
  color_t m_turn;
  bool m_has_ep_pawn;
  bool m_white_has_long_castle;
  bool m_white_has_short_castle;
  bool m_black_has_long_castle;
  bool m_black_has_short_castle;

  constexpr GameState(color_t white_move, bool has_ep_pawn,
                      bool white_has_long_castle, bool white_has_short_castle,
                      bool black_has_long_castle, bool black_has_short_castle)
      : m_turn(white_move),
        m_has_ep_pawn(has_ep_pawn),
        m_white_has_long_castle(white_has_long_castle),
        m_white_has_short_castle(white_has_short_castle),
        m_black_has_long_castle(black_has_long_castle),
        m_black_has_short_castle(black_has_short_castle) {}

  static constexpr GameState Default() {
    return {WHITE, false, true, true, true, true};
  }

  [[nodiscard]] inline constexpr color_t turn() const { return m_turn; }

  [[nodiscard]] inline constexpr bool isWhitesTurn() const {
    return (m_turn == WHITE);
  }

  [[nodiscard]] inline constexpr bool isBlacksTurn() const {
    return (m_turn == BLACK);
  }

  [[nodiscard]] inline constexpr bool hasEnPassant() const {
    return m_has_ep_pawn;
  }

  [[nodiscard]] inline constexpr bool whiteHasShortCastle() const {
    return m_white_has_short_castle;
  }

  [[nodiscard]] inline constexpr bool blackHasShortCastle() const {
    return m_black_has_short_castle;
  }

  [[nodiscard]] inline constexpr bool whiteHasLongCastle() const {
    return m_white_has_long_castle;
  }

  [[nodiscard]] inline constexpr bool blackHasLongCastle() const {
    return m_black_has_long_castle;
  }

  [[nodiscard]] inline constexpr bool has_long_castle() const {
    if (m_turn)
      return m_white_has_long_castle;
    else
      return m_black_has_long_castle;
  }

  [[nodiscard]] inline constexpr bool has_short_castle() const {
    if (m_turn)
      return m_white_has_short_castle;
    else
      return m_black_has_short_castle;
  };

  [[nodiscard]] inline GameState applyMove(const runtime_move& move) {
    return {!m_turn, false, m_white_has_long_castle,
                     m_white_has_short_castle,
                     m_black_has_long_castle, m_black_has_short_castle};
  }
};