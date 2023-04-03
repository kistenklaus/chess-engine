//
// Created by karl on 26/03/23.
//
#pragma once

#include <x86intrin.h>

#include <functional>

#include "Board.h"
#include "GameState.h"
#include "MagicLookup.h"
#include "banmask.h"
#include "bitmap.h"
#include "checkmask.h"
#include "move.h"
#include "pinmask.h"
#include "x86utils.h"

namespace movegen::shift {

compiletime bitmap_t PawnsNotLeft() { return ~Board::File8; }

compiletime bitmap_t PawnsNotRight() { return ~Board::File1; }

template <color_t turn> compiletime bitmap_t PawnForward(bitmap_t map) {
  if constexpr (turn == WHITE)
    return map << 8;
  else
    return map >> 8;
}

template <color_t turn> compiletime bitmap_t PawnForward2(bitmap_t map) {
  if constexpr (turn == WHITE)
    return map << 16;
  else
    return map >> 16;
}

template <color_t turn> compiletime bitmap_t PawnBackward(bitmap_t map) {
  return PawnForward<!turn>(map);
}

template <color_t turn> compiletime bitmap_t PawnBackward2(bitmap_t map) {
  return PawnForward2<!turn>(map);
}

template <color_t turn> compiletime bitmap_t PawnAttackLeft(bitmap_t map) {
  if constexpr (turn == WHITE)
    return map << 9;
  else
    return map >> 7;
}

template <color_t turn> compiletime bitmap_t PawnAttackRight(bitmap_t map) {
  if constexpr (turn == WHITE) {
    return map << 7;
  }else {
    return map >> 9;
  }
}

template <color_t turn> compiletime bitmap_t PawnUndoAttackLeft(bitmap_t map) {
  return PawnAttackRight<!turn>(map);
}

template <color_t turn> compiletime bitmap_t PawnUndoAttackRight(bitmap_t map) {
  return PawnAttackLeft<!turn>(map);
}

template <color_t turn>
force_inline void PawnPruneLeft(bitmap_t &pawn, const bitmap_t pinD12) {
  const bitmap_t pinned =
      pawn & PawnUndoAttackLeft<turn>(pinD12 & PawnsNotRight());
  const bitmap_t unpinned = pawn & ~pinD12;
  pawn = (pinned | unpinned);
}

template <color_t turn>
force_inline void PawnPruneRight(bitmap_t &pawn, const bitmap_t pinD12) {
  const bitmap_t pinned =
      pawn & PawnUndoAttackRight<turn>(pinD12 & PawnsNotLeft());
  const bitmap_t unpinned = pawn & ~pinD12;
  pawn = (pinned | unpinned);
}

template <color_t turn>
force_inline void PawnPrunePush(bitmap_t &pawn, const bitmap_t pinHV) {
  const bitmap_t pinned = pawn & PawnBackward<turn>(pinHV);
  const bitmap_t unpinned = pawn & ~pinHV;
  pawn = (pinned | unpinned);
}

template <color_t turn>
force_inline void PawnPruneDoublePush(bitmap_t &pawn, const bitmap_t pinHV) {
  const bitmap_t pinned = pawn & PawnBackward2<turn>(pinHV);
  const bitmap_t unpinned = pawn & ~pinHV;
  pawn = (pinned | unpinned);
}

template <color_t turn>
force_inline void PawnPruneLeftEP(bitmap_t &pawn, const bitmap_t pinD12) {
  const bitmap_t pinned =
      pawn & PawnUndoAttackLeft<turn>(pinD12 & PawnsNotRight());
  const bitmap_t unpinned = pawn & ~pinD12;
  pawn = (pinned | unpinned);
}

template <color_t turn>
force_inline void PawnPruneRightEP(bitmap_t &pawn, const bitmap_t pinD12) {
  const bitmap_t pinned =
      pawn & PawnUndoAttackRight<turn>(pinD12 & PawnsNotLeft());
  const bitmap_t unpinned = pawn & ~pinD12;
  pawn = (pinned | unpinned);
}

template <color_t turn> compiletime bitmap_t RelativeSecondRank() {
  if constexpr (turn == WHITE)
    return Board::Rank2;
  else
    return Board::Rank7;
}

template <color_t turn> compiletime bitmap_t RelativeSevenRank() {
  if constexpr (turn == WHITE)
    return Board::Rank7;
  else
    return Board::Rank2;
}

template <color_t turn> compiletime bitmap_t RelativeEnPassantRank() {
  if constexpr (turn == WHITE)
    return 0xFFull << 32;
  else
    return 0xFFull << 24;
}

template <color_t turn> compiletime bitmap_t RelativeLeftRookMask() {
  if constexpr (turn == WHITE) {
    return Board::WhiteLongCastleRookMask;
  } else {
    return Board::BlackLongCastleRookMask;
  }
}

template <color_t turn> compiletime bitmap_t RelativeRightRookMask() {
  if constexpr (turn == WHITE) {
    return Board::WhiteShortCastleRookMask;
  } else {
    return Board::BlackShortCastleRookMask;
  }
}
} // namespace movegen::shift
