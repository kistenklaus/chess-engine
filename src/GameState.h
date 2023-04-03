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
      : m_turn(white_move), m_has_ep_pawn(has_ep_pawn),
        m_white_has_long_castle(white_has_long_castle),
        m_white_has_short_castle(white_has_short_castle),
        m_black_has_long_castle(black_has_long_castle),
        m_black_has_short_castle(black_has_short_castle) {}

  static constexpr GameState Default() {
    return {WHITE, false, true, true, true, true};
  }

  [[nodiscard]] inline constexpr color_t turn() const { return m_turn; }

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

  [[nodiscard]] constexpr bool canCastle() const {
    if (m_turn)
      return m_white_has_long_castle | m_white_has_short_castle;
    else
      return m_black_has_long_castle | m_black_has_short_castle;
  }

  [[nodiscard]] constexpr bool canCastleShort() const {
    if (m_turn)
      return m_white_has_short_castle;
    else
      return m_black_has_short_castle;
  }

  [[nodiscard]] constexpr bool canCastleLong() const {
    if (m_turn)
      return m_white_has_long_castle;
    return m_black_has_long_castle;
  }
};

template <GameState state, figure figure, move_flag flag>
compiletime GameState compiletimeStateTransition() {
  if (figure == PAWN && flag == MOVE_FLAG_DOUBLE_PAWN_PUSH) {
    return {!state.turn(),
            true,
            state.whiteHasLongCastle(),
            state.whiteHasShortCastle(),
            state.blackHasLongCastle(),
            state.blackHasShortCastle()};
  }
  if (figure == KING) {
    if constexpr (state.turn() == WHITE) {
      return {!state.turn(),
              false,
              false,
              false,
              state.blackHasLongCastle(),
              state.blackHasShortCastle()};
    } else {
      return {!state.turn(),
              false,
              state.whiteHasLongCastle(),
              state.whiteHasShortCastle(),
              false,
              false};
    }
  }
  if (figure == ROOK && flag == MOVE_FLAG_LEFT_ROOK) {
    if constexpr (state.turn() == WHITE) {
      return {!state.turn(),
              false,
              false,
              state.whiteHasShortCastle(),
              state.blackHasLongCastle(),
              state.blackHasShortCastle()};
    } else {
      return {!state.turn(),
              false,
              state.whiteHasLongCastle(),
              state.whiteHasShortCastle(),
              false,
              state.blackHasShortCastle()};
    }
  }
  if constexpr (figure == ROOK && flag == MOVE_FLAG_RIGHT_ROOK) {
    if constexpr (state.turn() == WHITE) {
      return {!state.turn(),
              false,
              state.whiteHasLongCastle(),
              false,
              state.blackHasLongCastle(),
              state.blackHasShortCastle()};
    } else {
      return {!state.turn(),
              false,
              state.whiteHasLongCastle(),
              state.whiteHasShortCastle(),
              state.blackHasLongCastle(),
              false};
    }
  }

  return {!state.turn(),
          false,
          state.whiteHasLongCastle(),
          state.whiteHasShortCastle(),
          state.blackHasLongCastle(),
          state.blackHasShortCastle()};
}

inline GameState runtimeStateTransition(const GameState &state, figure figure,
                                        move_flag flag) {
  if (figure == PAWN && flag == MOVE_FLAG_DOUBLE_PAWN_PUSH) {
    return {!state.turn(),
            true,
            state.whiteHasLongCastle(),
            state.whiteHasShortCastle(),
            state.blackHasLongCastle(),
            state.blackHasShortCastle()};
  }
  if (figure == KING) {
    if (state.turn() == WHITE) {
      return {!state.turn(),
              false,
              false,
              false,
              state.blackHasLongCastle(),
              state.blackHasShortCastle()};
    } else {
      return {!state.turn(),
              false,
              state.whiteHasLongCastle(),
              state.whiteHasShortCastle(),
              false,
              false};
    }
  }
  if (figure == ROOK && flag == MOVE_FLAG_LEFT_ROOK) {
    if (state.turn() == WHITE) {
      return {!state.turn(),
              false,
              false,
              state.whiteHasShortCastle(),
              state.blackHasLongCastle(),
              state.blackHasShortCastle()};
    } else {
      return {!state.turn(),
              false,
              state.whiteHasLongCastle(),
              state.whiteHasShortCastle(),
              false,
              state.blackHasShortCastle()};
    }
  }
  if (figure == ROOK && flag == MOVE_FLAG_RIGHT_ROOK) {
    if (state.turn() == WHITE) {
      return {!state.turn(),
              false,
              state.whiteHasLongCastle(),
              false,
              state.blackHasLongCastle(),
              state.blackHasShortCastle()};
    } else {
      return {!state.turn(),
              false,
              state.whiteHasLongCastle(),
              state.whiteHasShortCastle(),
              state.blackHasLongCastle(),
              false};
    }
  }

  return {!state.turn(),
          false,
          state.whiteHasLongCastle(),
          state.whiteHasShortCastle(),
          state.blackHasLongCastle(),
          state.blackHasShortCastle()};
}
