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

namespace movegen::util {

template <GameState state>
compiletime bitmap_t canCastleLeft(bitmap_t attacked, bitmap_t occupied,
                                   bitmap_t rook, const Board &board) {
  if (state.turn() && state.whiteHasShortCastle()) {
    if (occupied & Board::WhiteShortCastleMoveMask) return false;
    if (attacked & Board::WhiteShortCastleAttackMask) return false;
    if (rook & Board::WhiteShortCastleRookMask) return true;
  } else if (state.blackHasShortCastle()) {
    if (occupied & Board::BlackShortCastleMoveMask) return false;
    if (attacked & Board::BlackShortCastleAttackMask) return false;
    if (rook & Board::BlackShortCastleRookMask) return true;
  }
  return false;
}

template <GameState state>
compiletime bitmap_t canCastleRight(bitmap_t attacked, bitmap_t occupied,
                                    bitmap_t rook, const Board &board) {
  if (state.turn() && state.whiteHasLongCastle()) {
    if (occupied & Board::WhiteLongCastleMoveMask) return false;
    if (attacked & Board::WhiteLongCastleAttackMask) return false;
    if (rook & Board::WhiteLongCastleRookMask) return true;
  } else if (state.blackHasLongCastle()) {
    if (occupied & Board::BlackLongCastleMoveMask) return false;
    if (attacked & Board::BlackLongCastleAttackMask) return false;
    if (rook & Board::BlackLongCastleRookMask) return true;
  }
  return false;
}

}