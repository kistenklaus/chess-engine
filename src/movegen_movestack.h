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
#include "movegen_shift.h"

namespace movegen::Movestack {

static const int MAX_DEPTH = 32;
static inline bitmap_t g_KingAttack[MAX_DEPTH];
static inline bitmap_t g_EnemyKingAttack[MAX_DEPTH];
static inline checkmask_t g_Checkmask[MAX_DEPTH];

static inline bitmap_t g_EnPassantTarget = {};
static inline bitmap_t g_RookPin = {};
static inline bitmap_t g_BishopPin = {};

template <GameState state, int depth>
void init(const Board &board, bitmap_t epTarget) {
  using namespace shift;
  g_EnPassantTarget = epTarget;
  constexpr auto turn = state.turn();

  const bitmap_t king = board.King<turn>();
  const bitmap_t kingTile = SQUARE_OF(king);

  const bitmap_t enemyKing = board.King<!turn>();
  const bitmap_t enemyKingTile = SQUARE_OF(enemyKing);

  g_KingAttack[depth] = MagicLookup::King(kingTile);
  g_EnemyKingAttack[depth] = MagicLookup::King(enemyKingTile);

  // Calculate checks from pawns.
  {
    const bitmap_t leftAttackingPawns =
        PawnAttackLeft<!turn>(board.Pawns<!turn>() & shift::PawnsNotLeft());
    const bitmap_t rightAttackingPawns =
        PawnAttackRight<!turn>(board.Pawns<!turn>() & shift::PawnsNotRight());

    if (leftAttackingPawns & king) {
      g_Checkmask[depth] = PawnAttackRight<turn>(king);
    } else if (rightAttackingPawns & king) {
      g_Checkmask[depth] = PawnAttackLeft<turn>(king);
    } else {
      g_Checkmask[depth] = 0xFFFFFFFFFFFFFFFFull;
    }
  }
  // Calculate check from enemy knights
  {
    bitmap_t knightChecks =
        MagicLookup::Knight(kingTile) & board.Knights<!turn>();
    if (knightChecks) g_Checkmask[depth] = knightChecks;
  }
  // We are assuming only normal positions to arise.
  // So no checks by two knights/pawns at once.
  // There HAS to be a slider involved in doublecheck.
  // No two knights or pawn/knight can check at once.
}

template <GameState state>
force_inline void registerHVPin(bitmap_t kingTile, bitmap_t enemyTile,
                                const Board &board) {
  const bitmap_t pinmask = MagicLookup::PinBetween(kingTile, enemyTile);
  if (pinmask & board.OccupiedBy<state.turn()>()) {
    g_RookPin |= pinmask;
  }
}

template <GameState state>
force_inline void registerD12Pin(bitmap_t kingTile, bitmap_t enemyTile,
                                 const Board &board,
                                 bitmap_t &enPassantTarget) {
  const bitmap_t pinmask = MagicLookup::PinBetween(kingTile, enemyTile);
  if constexpr (state.hasEnPassant()) {
    if (pinmask & enPassantTarget) {
      enPassantTarget = 0;
    }
  }
  if (pinmask & board.OccupiedBy<state.turn()>()) {
    g_BishopPin |= pinmask;
  }
}

template <GameState state>
force_inline void registerEnPassantPin(bitmap_t kingTile, bitmap_t king,
                                       bitmap_t enemyHVSliders,
                                       const Board &board) {
  using namespace shift;
  constexpr auto turn = state.turn();
  const bitmap_t pawns = board.Pawns<state.turn()>();
  if ((RelativeEnPassantRank<turn>() & king) &&
      (RelativeEnPassantRank<turn>() & enemyHVSliders) &&
      (RelativeEnPassantRank<turn>() & pawns)) {
    bitmap_t epLeftPawn = pawns & ((g_EnPassantTarget & shift::PawnsNotRight()
    ) >> 1);
    bitmap_t epRightPawn = pawns & ((g_EnPassantTarget & shift::PawnsNotLeft()
    ) <<
      1);

    if (epLeftPawn) {
      bitmap_t afterEpOcc =
          board.Occupied() & ~(g_EnPassantTarget | epLeftPawn);
      if (MagicLookup::Rook(kingTile, afterEpOcc) &
          RelativeEnPassantRank<turn>() & enemyHVSliders) {
        g_EnPassantTarget = 0;
      }
    }
    if (epRightPawn) {
      bitmap_t afterEpOcc =
          board.Occupied() & ~(g_EnPassantTarget | epRightPawn);
      if (MagicLookup::Rook(kingTile, afterEpOcc) &
          RelativeEnPassantRank<turn>() & enemyHVSliders) {
        g_EnPassantTarget = 0;
      }
    }
  }
}

force_inline void checkBySlider(bitmap_t kingTile, bitmap_t enemyTile,
                                banmask_t &banmask, checkmask_t &checkmask) {
  if (checkmask == 0xFFFFFFFFFFFFFFFFull) {
    checkmask = MagicLookup::PinBetween(kingTile, enemyTile);
  } else {
    checkmask = 0;
  }
  banmask |= MagicLookup::CheckBetween(kingTile, enemyTile);
}

template <GameState state, int depth>
force_inline bitmap_t refreshStack(const Board &board, banmask_t &banmask,
                                   checkmask_t &checkmask) {
  using namespace shift;
  constexpr bool turn = state.turn();
  const bitmap_t king = board.King<turn>();
  const bitmap_t kingTile = SQUARE_OF(king);
  // Pinned pieces + checks by sliders
  {
    g_RookPin = 0;
    g_BishopPin = 0;
    if (MagicLookup::RookMask(kingTile) & board.EnemyHVSliders<turn>()) {
      bitmap_t attackingHVSliders =
          MagicLookup::Rook(kingTile, board.Occupied()) &
          board.EnemyHVSliders<turn>();
      WHILE_RESET_LSB(attackingHVSliders) {
        bitmap_t hvSliderTile = SQUARE_OF(attackingHVSliders);
        checkBySlider(kingTile, hvSliderTile, banmask, checkmask);
      }
      bitmap_t pinningHVSliders =
          MagicLookup::RookXRay(kingTile, board.Occupied()) &
          board.EnemyHVSliders<turn>();
      WHILE_RESET_LSB(pinningHVSliders) {
        const bitmap_t hvSliderTile = SQUARE_OF(pinningHVSliders);
        registerHVPin<state>(kingTile, hvSliderTile, board);
      }
    }
    if (MagicLookup::BishopMask(kingTile) & board.EnemyD12Sliders<turn>()) {
      bitmap_t attackingD12Sliders =
          MagicLookup::Bishop(kingTile, board.Occupied()) &
          board.EnemyD12Sliders<turn>();
      WHILE_RESET_LSB(attackingD12Sliders) {
        const bitmap_t d12SliderTile = SQUARE_OF(attackingD12Sliders);
        checkBySlider(kingTile, d12SliderTile, banmask, checkmask);
      }
      bitmap_t pinningD12Sliders =
          MagicLookup::BishopXRay(kingTile, board.Occupied()) &
          board.EnemyD12Sliders<turn>();
      WHILE_RESET_LSB(pinningD12Sliders) {
        const bitmap_t d12SliderTile = SQUARE_OF(pinningD12Sliders);
        registerD12Pin<state>(kingTile, d12SliderTile, board,
                              g_EnPassantTarget);
      }
    }
    if constexpr (state.hasEnPassant()) {
      registerEnPassantPin<state>(kingTile, king, board.EnemyHVSliders<turn>(),
                                  board);
    }
  }
  Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
  bitmap_t kingAttack = Movestack::g_KingAttack[depth] &
                        board.EnemyOrEmpty<state.turn()>() & ~banmask;
  if (kingAttack == 0) return 0;

  // Calculate Enemy Knight - keep this first
  {
    bitmap_t knights = board.Knights<!turn>();
    WHILE_RESET_LSB(knights) {
      const bitmap_t knightTile = SQUARE_OF(knights);
      banmask |= MagicLookup::Knight(knightTile);
    }
  }
  // Calculate check from enemy pawns
  {
    const bitmap_t pawns = board.Pawns<!turn>();
    const bitmap_t pawnAttacksLeft =
        PawnAttackLeft<!turn>(pawns) & shift::PawnsNotLeft();
    const bitmap_t pawnAttacksRight =
        PawnAttackRight<!turn>(pawns) & shift::PawnsNotRight();
    banmask |= pawnAttacksLeft | pawnAttacksRight;
  }
  // Calculate enemy bishops
  {
    bitmap_t bishops = board.EnemyD12Sliders<turn>();
    WHILE_RESET_LSB(bishops) {
      const bitmap_t bishopTile = SQUARE_OF(bishops);
      bitmap_t atk = MagicLookup::Bishop(bishopTile, board.Occupied());
      banmask |= atk;
    }
  }
  {
    bitmap_t rooks = board.EnemyHVSliders<turn>();
    WHILE_RESET_LSB(rooks) {
      const bitmap_t rookTile = SQUARE_OF(rooks);
      bitmap_t atk = MagicLookup::Rook(rookTile, board.Occupied());
      banmask |= atk;
    }
  }
  return kingAttack & ~banmask;
}

}