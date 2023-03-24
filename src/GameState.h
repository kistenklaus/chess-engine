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
};

template<GameState state, compiletime_move_flag flag>
inline constexpr GameState compiletimeStateTransition(){
  if constexpr (flag == MOVE_COMPILETIME_FLAG_LOSE_SHORT_CASTLE) {
    if constexpr (state.turn()) {
      return {!state.turn(),
              false,
              state.whiteHasShortCastle(),
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
  } else if constexpr (flag == MOVE_COMPILETIME_FLAG_LOSE_LONG_CASTLE) {
    if constexpr (state.turn()) {
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
              state.whiteHasLongCastle(),
              false,
              state.whiteHasShortCastle()};
    }
  } else if constexpr (flag == MOVE_COMPILETIME_FLAG_DOUBLE_PAWN_PUSH) {
    return {!state.turn(),
            true,
            state.whiteHasLongCastle(),
            state.whiteHasLongCastle(),
            state.blackHasLongCastle(),
            state.blackHasShortCastle()};
  } else {
    return {!state.turn(),
            false,
            state.whiteHasLongCastle(),
            state.whiteHasLongCastle(),
            state.blackHasLongCastle(),
            state.blackHasShortCastle()};
  }
}

inline constexpr GameState runtimeStateTransition(const GameState& state,
                                                  runtime_move move){
  const compiletime_move_flag& flag = move.m_compiletimeFlag;
  if (flag == MOVE_COMPILETIME_FLAG_LOSE_SHORT_CASTLE) {
    if (state.turn()) {
      return {!state.turn(),
              false,
              state.whiteHasShortCastle(),
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
  } else if (flag == MOVE_COMPILETIME_FLAG_LOSE_LONG_CASTLE) {
    if (state.turn()) {
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
              state.whiteHasLongCastle(),
              false,
              state.whiteHasShortCastle()};
    }
  } else if (flag == MOVE_COMPILETIME_FLAG_DOUBLE_PAWN_PUSH) {
    return {!state.turn(),
            true,
            state.whiteHasLongCastle(),
            state.whiteHasLongCastle(),
            state.blackHasLongCastle(),
            state.blackHasShortCastle()};
  } else {
    return {!state.turn(),
            false,
            state.whiteHasLongCastle(),
            state.whiteHasLongCastle(),
            state.blackHasLongCastle(),
            state.blackHasShortCastle()};
  }
}
