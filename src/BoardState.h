#pragma once
#include "color.h"
#include "figure.h"
#include "move.h"

class BoardState {
 public:
  const color_t m_turn;
  const bool _has_ep_pawn;
  const bool m_white_has_long_castle;
  const bool m_white_has_short_castle;
  const bool m_black_has_long_castle;
  const bool m_black_has_short_castle;

  constexpr BoardState(color_t white_move, bool has_ep_pawn,
                       bool white_has_long_castle, bool white_has_short_castle,
                       bool black_has_long_castle, bool black_has_short_castle)
      : m_turn(white_move),
        _has_ep_pawn(has_ep_pawn),
        m_white_has_long_castle(white_has_long_castle),
        m_white_has_short_castle(white_has_short_castle),
        m_black_has_long_castle(black_has_long_castle),
        m_black_has_short_castle(black_has_short_castle) {}

  static constexpr BoardState Default() {
    return {WHITE, false, true, true, true, true};
  }

  [[nodiscard]] inline constexpr color_t turn() const { return m_turn; }

  [[nodiscard]] inline constexpr bool isWhitesTurn() const {
    return (m_turn == WHITE);
  }

  [[nodiscard]] inline constexpr bool isBlacksTurn() const {
    return (m_turn == BLACK);
  }

  /*
  template <figure_type type>
  [[nodiscard]] inline constexpr BoardState nextTurn(move_t move) {
    static_assert(type != EMPTY);
    if constexpr (type == PAWN) {
      return nextPawnTurn(move);
    } else if constexpr (type == BISHOP) {
      return nextBishopTurn(move);
    } else if constexpr (type == KNIGHT) {
      return nextKnightTurn(move);
    } else if constexpr (type == ROOK) {
      return nextRookTurn(move);
    } else if constexpr (type == QUEEN) {
      return nextQueenTurn(move);
    } else if constexpr (type == KING) {
      return nextKingTurn(move);
    }
    throw std::invalid_argument("invalid figure type")
  }

  [[nodiscard]] inline constexpr BoardState nextPawnTurn(move_t move) {
    return *this;
  }

  [[nodiscard]] inline constexpr BoardState nextBishopTurn(move_t move) {
    return *this;
  }

  [[nodiscard]] inline constexpr BoardState nextKnightTurn(move_t move) {
    return *this;
  }

  [[nodiscard]] inline constexpr BoardState nextRookTurn(move_t move) {
    return *this;
  }

  [[nodiscard]] inline constexpr BoardState nextQueenTurn(move_t move) {
    return *this;
  }

  [[nodiscard]] inline constexpr BoardState nextKingTurn(move_t move) {
    return *this;
  }
   */

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
  }

  [[nodiscard]] inline constexpr BoardState register_short_rook_move() const {
    if (m_turn) {
      return {!m_turn,
              false,
              m_white_has_long_castle,
              false,
              m_black_has_long_castle,
              m_black_has_short_castle};
    } else {
      return {!m_turn,
              false,
              m_white_has_long_castle,
              m_white_has_short_castle,
              m_black_has_long_castle,
              false};
    }
  }

  [[nodiscard]] inline constexpr BoardState register_long_rook_move() const {
    if (m_turn) {
      return {!m_turn,
              false,
              false,
              m_white_has_short_castle,
              m_black_has_long_castle,
              m_black_has_short_castle};
    } else {
      return {!m_turn,
              false,
              m_white_has_long_castle,
              m_white_has_short_castle,
              false,
              m_black_has_short_castle};
    }
  }

  [[nodiscard]] inline constexpr BoardState register_king_move() const {
    if (m_turn) {
      return {!m_turn,
              false,
              false,
              false,
              m_black_has_long_castle,
              m_black_has_short_castle};
    } else {
      return {!m_turn, false, m_white_has_long_castle, m_white_has_short_castle,
              false,   false};
    }
  }

  [[nodiscard]] inline constexpr BoardState register_move() const {
    return {!m_turn,
            false,
            m_white_has_long_castle,
            m_white_has_short_castle,
            m_black_has_long_castle,
            m_black_has_short_castle};
  }
};
