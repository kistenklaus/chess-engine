#pragma once

#include <x86intrin.h>

#include <functional>

#include "Board.h"
#include "GameState.h"
#include "MagicLookup.h"
#include "banmask.h"
#include "banmask_generation.h"
#include "bitmap.h"
#include "checkmask.h"
#include "checkmask_generation.h"
#include "lookup.h"
#include "move.h"
#include "pinmask.h"
#include "pinmask_generation.h"
#include "x86utils.h"

namespace Movegen {

namespace internal {

compiletime bitmap_t PawnsNotLeft() { return ~Board::File1; }

compiletime bitmap_t PawnsNotRight() { return ~Board::File8; }

template <color_t turn>
compiletime bitmap_t PawnForward(bitmap_t map) {
  if constexpr (turn == WHITE)
    return map << 8;
  else
    return map >> 8;
}

template <color_t turn>
compiletime bitmap_t PawnForward2(bitmap_t map) {
  if constexpr (turn == WHITE)
    return map << 16;
  else
    return map >> 16;
}

template <color_t turn>
compiletime bitmap_t PawnBackward(bitmap_t map) {
  return PawnForward<!turn>(map);
}

template <color_t turn>
compiletime bitmap_t PawnBackward2(bitmap_t map) {
  return PawnForward2<!turn>(map);
}

template <color_t turn>
compiletime bitmap_t PawnAttackLeft(bitmap_t map) {
  if constexpr (turn == WHITE)
    return map << 9;
  else
    return map >> 7;
}

template <color_t turn>
compiletime bitmap_t PawnAttackRight(bitmap_t map) {
  if constexpr (turn == WHITE)
    return map << 7;
  else
    return map >> 9;
}

template <color_t turn>
compiletime bitmap_t PawnUndoAttackLeft(bitmap_t map) {
  return PawnAttackRight<!turn>(map);
}

template <color_t turn>
compiletime bitmap_t PawnUndoAttackRight(bitmap_t map) {
  return PawnAttackLeft<!turn>(map);
}

template <color_t turn>
force_inline void pawnPruneLeft(bitmap_t &pawn, const bitmap_t pinD12) {
  const bitmap_t pinned =
      pawn & PawnUndoAttackLeft<turn>(pinD12 & PawnsNotRight());
  const bitmap_t unpinned = pawn & ~pinD12;
  pawn = (pinned | unpinned);
}

template <color_t turn>
force_inline void pawnPruneRight(bitmap_t &pawn, const bitmap_t pinD12) {
  const bitmap_t pinned =
      pawn & PawnUndoAttackRight<turn>(pinD12 & PawnsNotLeft());
  const bitmap_t unpinned = pawn & ~pinD12;
  pawn = (pinned | unpinned);
}

template <color_t turn>
force_inline void pawnPrunePush(bitmap_t &pawn, const bitmap_t pinHV) {
  const bitmap_t pinned = pawn & PawnBackward<turn>(pinHV);
  const bitmap_t unpinned = pawn & ~pinHV;
  pawn = (pinned | unpinned);
}

template <color_t turn>
force_inline void pawnPruneDoublePush(bitmap_t &pawn, const bitmap_t pinHV) {
  const bitmap_t pinned = pawn & PawnBackward2<turn>(pinHV);
  const bitmap_t unpinned = pawn & ~pinHV;
  pawn = (pinned | unpinned);
}

template <color_t turn>
force_inline void pawnPruneLeftEP(bitmap_t &pawn, const bitmap_t pinD12) {
  const bitmap_t pinned =
      pawn & PawnUndoAttackLeft<turn>(pinD12 & PawnsNotRight());
  const bitmap_t unpinned = pawn & ~pinD12;
  pawn = (pinned | unpinned);
}

template <color_t turn>
force_inline void pawnPruneRightEP(bitmap_t &pawn, const bitmap_t pinD12) {
  const bitmap_t pinned =
      pawn & PawnUndoAttackRight<turn>(pinD12 & PawnsNotLeft());
  const bitmap_t unpinned = pawn & ~pinD12;
  pawn = (pinned | unpinned);
}

template <color_t turn>
compiletime bitmap_t RelativeFirstRank() {
  if constexpr (turn == WHITE)
    return Board::Rank2;
  else
    return Board::Rank7;
}

template <color_t turn>
compiletime bitmap_t RelativeSecondLastRank() {
  if constexpr (turn == WHITE)
    return Board::Rank7;
  else
    return Board::Rank2;
}

template <color_t turn>
compiletime bitmap_t RelativeEnPassantRank() {
  if constexpr (turn == WHITE)
    return 0xFFull << 32;
  else
    return 0xFFull << 24;
}

template <color_t turn>
compiletime bitmap_t RelativeLeftRookMask() {
  if constexpr (turn == WHITE) {
    return Board::WhiteLongCastleRookMask;
  } else {
    return Board::BlackLongCastleRookMask;
  }
}

template <color_t turn>
compiletime bitmap_t RelativeRightRookMask() {
  if constexpr (turn == WHITE) {
    return Board::WhiteShortCastleRookMask;
  } else {
    return Board::BlackShortCastleRookMask;
  }
}

template <GameState state>
compiletime bitmap_t canCastleLeft(bitmap_t attacked, bitmap_t occupied,
                                   bitmap_t rook, const Board &board) {
  if (state.turn() && state.whiteHasShortCastle()) {
    if (occupied & Board::WhiteShortCastleMoveMask) return false;
    if (attacked & Board::WhiteShortCastleAttackMask) return false;
    if (rook & Board::WhiteShortCastleRookMask) return true;
    return false;
  } else if (state.blackHasShortCastle()) {
    if (occupied & Board::BlackShortCastleMoveMask) return false;
    if (attacked & Board::BlackShortCastleAttackMask) return false;
    if (rook & Board::BlackShortCastleRookMask) return true;
    return false;
  }
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

namespace Movestack {
static const int MAX_DEPTH = 32;
static inline bitmap_t g_KingAttack[MAX_DEPTH];
static inline bitmap_t g_EnemyKingAttack[MAX_DEPTH];
static inline checkmask_t g_Checkmask[MAX_DEPTH];

template <GameState state, int depth>
force_inline void init(const Board &board) {
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
        PawnAttackLeft<!turn>(board.Pawns<!turn>() & PawnsNotLeft());
    const bitmap_t rightAttackingPawns =
        PawnAttackRight<!turn>(board.Pawns<!turn>() & PawnsNotRight());

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

}  // namespace Movestack

bitmap_t g_EnPassantTarget = {};
bitmap_t g_RookPin = {};
bitmap_t g_BishopPin = {};

force_inline void initMovegen(bitmap_t epInit) { g_EnPassantTarget = epInit; }

template <class GameState state, typename MoveReceiver, int depth>
inline void enumerateMoves(const Board &board, MoveReceiver &receiver,
                           bitmap_t kingAttack, checkmask_t checkmask,
                           banmask_t banmask) {
  constexpr color_t turn = state.turn();
  const bool noCheck = (checkmask == 0xFFFFFFFFFFFFFFFFull);

  const pinmask_t pinmask(g_RookPin, g_BishopPin);
  const bitmap_t movable = board.EnemyOrEmpty<turn>() & checkmask;
  const bitmap_t epTarget = g_EnPassantTarget;

  // Kingmoves
  {
    bitmap_t king = board.King<turn>();
    WHILE_RESET_LSB(kingAttack) {
      const bitmap_t targetTile = SQUARE_OF(kingAttack);
      Movestack::g_EnemyKingAttack[depth - 1] = MagicLookup::King(targetTile);
      const bitmap_t target = 1ull << targetTile;
      if (target & board.OccupiedBy<!turn>()) {
        receiver.template move<KING, MOVE_FLAG_CAPTURE>(king, target);
      } else {
        receiver.template move<KING, MOVE_FLAG_SILENT>(king, target);
      }
    }
    // Castling
    if constexpr (state.canCastleLong()) {
      if (noCheck && canCastleLeft<state>(banmask, board.Occupied(),
                                          board.Rooks<turn>(), board)) {
        Movestack::g_EnemyKingAttack[depth - 1] =
            MagicLookup::King(SQUARE_OF(king << 2));
        receiver.template move<KING, MOVE_FLAG_LONG_CASTLE>(turn, turn << 2);
      }
    }
    if constexpr (state.canCastleShort()) {
      if (noCheck && canCastleRight<state>(banmask, board.Occupied(),
                                           board.Occupied(), board)) {
        Movestack::g_EnemyKingAttack[depth - 1] = MagicLookup::King(king >> 2);
        receiver.template move<KING, MOVE_FLAG_SHORT_CASTLE>(king, king >> 2);
      }
    }
    Movestack::g_EnemyKingAttack[depth - 1] = Movestack::g_KingAttack[depth];
  }
  // Pawnmoves
  {
    const bitmap_t pawnsD12 = board.Pawns<turn>() & ~pinmask.hv;
    const bitmap_t pawnsHV = board.Pawns<turn>() & ~pinmask.d12;

    bitmap_t leftAttackingPawns =
        pawnsD12 & PawnUndoAttackLeft<turn>(board.OccupiedBy<!turn>() &
                                            PawnsNotRight() & checkmask);
    bitmap_t rightAttackingPawns =
        pawnsD12 & PawnUndoAttackRight<turn>(board.OccupiedBy<!turn>() &
                                             PawnsNotLeft() & checkmask);

    bitmap_t pushablePawns = pawnsHV & PawnBackward<turn>(board.NotOccupied());

    bitmap_t doublePushablePawns =
        pushablePawns & RelativeFirstRank<turn>() &
        PawnBackward2<turn>(board.NotOccupied() & checkmask);
    pushablePawns &= PawnBackward<turn>(checkmask);

    pawnPruneLeft<turn>(leftAttackingPawns, pinmask.d12);
    pawnPruneRight<turn>(rightAttackingPawns, pinmask.d12);
    pawnPrunePush<turn>(pushablePawns, pinmask.hv);
    pawnPruneDoublePush<turn>(doublePushablePawns, pinmask.hv);

    if constexpr (state.hasEnPassant()) {
      bitmap_t epLeftPawn =
          pawnsD12 & PawnsNotLeft() & ((epTarget & checkmask) >> 1);
      bitmap_t epRightPawn =
          pawnsD12 & PawnsNotRight() & ((epTarget & checkmask) << 1);

      if (epLeftPawn | epRightPawn) {
        pawnPruneLeftEP<turn>(epLeftPawn, pinmask.d12);
        pawnPruneRightEP<turn>(epRightPawn, pinmask.d12);
        if (epLeftPawn) {
          receiver.template move<PAWN, MOVE_FLAG_EN_PASSANT>(
              epLeftPawn, PawnAttackLeft<turn>(epLeftPawn));
          receiver.template move<PAWN, MOVE_FLAG_EN_PASSANT>(
              epRightPawn, PawnAttackRight<turn>(epRightPawn));
        }
      }
    }
    if ((leftAttackingPawns | rightAttackingPawns | pushablePawns) &
        RelativeSecondLastRank<turn>()) {
      bitmap_t promoteLeft =
          leftAttackingPawns & RelativeSecondLastRank<turn>();
      bitmap_t promoteRight =
          rightAttackingPawns & RelativeSecondLastRank<turn>();
      bitmap_t promotePush = pushablePawns & RelativeSecondLastRank<turn>();

      bitmap_t noPromoteLeft =
          leftAttackingPawns & RelativeSecondLastRank<turn>();
      bitmap_t noPromoteRight =
          rightAttackingPawns & RelativeSecondLastRank<turn>();
      bitmap_t noPromotePush = pushablePawns & RelativeSecondLastRank<turn>();

      while (promoteLeft) {
        const bitmap_t pos = popBit(promoteLeft);
        receiver.template move<KNIGHT, MOVE_FLAG_PROMOTE>(
            pos, PawnAttackLeft<turn>(pos));
        receiver.template move<BISHOP, MOVE_FLAG_PROMOTE>(
            pos, PawnAttackLeft<turn>(pos));
        receiver.template move<ROOK, MOVE_FLAG_PROMOTE>(
            pos, PawnAttackLeft<turn>(pos));
        receiver.template move<QUEEN, MOVE_FLAG_PROMOTE>(
            pos, PawnAttackLeft<turn>(pos));
      }
      while (promoteRight) {
        const bitmap_t pos = popBit(promoteRight);
        receiver.template move<KNIGHT, MOVE_FLAG_PROMOTE>(
            pos, PawnAttackRight<turn>(pos));
        receiver.template move<BISHOP, MOVE_FLAG_PROMOTE>(
            pos, PawnAttackRight<turn>(pos));
        receiver.template move<ROOK, MOVE_FLAG_PROMOTE>(
            pos, PawnAttackRight<turn>(pos));
        receiver.template move<QUEEN, MOVE_FLAG_PROMOTE>(
            pos, PawnAttackRight<turn>(pos));
      }
      while (promotePush) {
        const bitmap_t pos = popBit(promotePush);
        receiver.template move<KNIGHT, MOVE_FLAG_PROMOTE>(
            pos, PawnForward<turn>(pos));
        receiver.template move<BISHOP, MOVE_FLAG_PROMOTE>(
            pos, PawnForward<turn>(pos));
        receiver.template move<ROOK, MOVE_FLAG_PROMOTE>(pos,
                                                        PawnForward<turn>(pos));
        receiver.template move<QUEEN, MOVE_FLAG_PROMOTE>(
            pos, PawnForward<turn>(pos));
      }
      while (noPromoteLeft) {
        const bitmap_t pos = popBit(noPromoteLeft);
        receiver.template move<PAWN, MOVE_FLAG_CAPTURE>(
            pos, PawnAttackLeft<turn>(pos));
      }
      while (noPromoteRight) {
        const bitmap_t pos = popBit(noPromoteRight);
        receiver.template move<PAWN, MOVE_FLAG_CAPTURE>(
            pos, PawnAttackRight<turn>(pos));
      }
      while (noPromotePush) {
        const bitmap_t pos = popBit(noPromotePush);
        receiver.template move<PAWN, MOVE_FLAG_SILENT>(pos,
                                                       PawnForward<turn>(pos));
      }
      while (doublePushablePawns) {
        const bitmap_t pos = popBit(doublePushablePawns);
        receiver.template move<PAWN, MOVE_FLAG_DOUBLE_PAWN_PUSH>(
            pos, PawnForward2<turn>(pos));
      }

    } else {
      while (leftAttackingPawns) {
        const bitmap_t pos = popBit(leftAttackingPawns);
        receiver.template move<PAWN, MOVE_FLAG_CAPTURE>(
            pos, PawnAttackLeft<turn>(pos));
      }
      while (rightAttackingPawns) {
        const bitmap_t pos = popBit(rightAttackingPawns);
        receiver.template move<PAWN, MOVE_FLAG_CAPTURE>(
            pos, PawnAttackRight<turn>(pos));
      }
      while (pushablePawns) {
        const bitmap_t pos = popBit(pushablePawns);
        receiver.template move<PAWN, MOVE_FLAG_SILENT>(pos,
                                                       PawnForward<turn>(pos));
      }
      while (doublePushablePawns) {
        const bitmap_t pos = popBit(doublePushablePawns);
        receiver.template move<PAWN, MOVE_FLAG_DOUBLE_PAWN_PUSH>(
            pos, PawnForward2<turn>(pos));
      }
    }
  }
  // Knightmoves
  {
    bitmap_t knights = board.Knights<turn>() & ~(pinmask.hv | pinmask.d12);
    WHILE_RESET_LSB(knights) {
      const bitmap_t tile = SQUARE_OF(knights);
      bitmap_t origin = 1ull << tile;
      bitmap_t moves = KnightLookUpTable::get()[tile] & movable;
      while (moves) {
        const bitmap_t target = popBit(moves);
        if (target & board.OccupiedBy<!turn>()) {
          receiver.template move<KNIGHT, MOVE_FLAG_CAPTURE>(origin, target);
        } else {
          receiver.template move<KNIGHT, MOVE_FLAG_SILENT>(origin, target);
        }
      }
    }
  }
  // Bishopmoves
  const bitmap_t queens = board.Queens<turn>();
  {
    bitmap_t bishops = board.Bishops<turn>() & ~pinmask.hv;

    bitmap_t bishopsPinned = (bishops | queens) & pinmask.d12;
    bitmap_t bishopsUnpinned = bishops & ~pinmask.d12;
    WHILE_RESET_LSB(bishopsPinned) {
      const bitmap_t tile = SQUARE_OF(bishopsPinned);
      bitmap_t move =
          MagicLookup::Bishop(tile, board.Occupied()) & movable & pinmask.d12;
      bitmap_t origin = 1ull << tile;
      if (origin & queens) {
        while (move) {
          const bitmap_t target = popBit(move);
          if (target & board.OccupiedBy<!turn>()) {
            receiver.template move<QUEEN, MOVE_FLAG_CAPTURE>(origin, target);
          } else {
            receiver.template move<QUEEN, MOVE_FLAG_SILENT>(origin, target);
          }
        }
      } else {
        while (move) {
          const bitmap_t target = popBit(move);
          if (target & board.OccupiedBy<!turn>()) {
            receiver.template move<BISHOP, MOVE_FLAG_CAPTURE>(origin, target);
          } else {
            receiver.template move<BISHOP, MOVE_FLAG_SILENT>(origin, target);
          }
        }
      }
    }
    WHILE_RESET_LSB(bishopsUnpinned) {
      const bitmap_t tile = SQUARE_OF(bishopsUnpinned);
      bitmap_t origin = 1ull << tile;
      bitmap_t move = MagicLookup::Bishop(tile, board.Occupied()) & movable;
      while (move) {
        const bitmap_t target = popBit(move);
        if (target & board.OccupiedBy<!turn>()) {
          receiver.template move<BISHOP, MOVE_FLAG_CAPTURE>(origin, target);
        } else {
          receiver.template move<BISHOP, MOVE_FLAG_SILENT>(origin, target);
        }
      }
    }
  }
  {
    bitmap_t rooks = board.Rooks<turn>() & ~pinmask.d12;
    bitmap_t rooksPinned = (rooks | queens) & pinmask.hv;
    bitmap_t rooksUnpinned = rooks & ~pinmask.hv;
    WHILE_RESET_LSB(rooksPinned) {
      const bitmap_t tile = SQUARE_OF(rooksPinned);
      bitmap_t move =
          MagicLookup::Rook(tile, board.Occupied()) & movable & pinmask.hv;
      bitmap_t origin = 1ull << tile;
      if (origin & queens) {
        while (move) {
          const bitmap_t target = popBit(move);
          if (target & board.OccupiedBy<!turn>()) {
            receiver.template move<QUEEN, MOVE_FLAG_CAPTURE>(origin, target);
          } else {
            receiver.template move<QUEEN, MOVE_FLAG_SILENT>(origin, target);
          }
        }
      } else {
        while (move) {
          const bitmap_t target = popBit(move);
          if (target & board.OccupiedBy<!turn>()) {
            receiver.template move<QUEEN, MOVE_FLAG_CAPTURE>(origin, target);
          } else {
            receiver.template move<QUEEN, MOVE_FLAG_SILENT>(origin, target);
          }
        }
      }
    }
    WHILE_RESET_LSB(rooksUnpinned) {
      const bitmap_t tile = SQUARE_OF(rooksUnpinned);
      bitmap_t move = MagicLookup::Rook(tile, board.Occupied()) & movable;
      bitmap_t origin = 1ull << tile;
      while (move) {
        const bitmap_t target = popBit(move);
        if (origin & RelativeLeftRookMask<turn>()) {
          receiver.template move<QUEEN, MOVE_FLAG_LEFT_ROOK>(origin, target);
        } else if (origin & RelativeRightRookMask<turn>()) {
          receiver.template move<QUEEN, MOVE_FLAG_RIGHT_ROOK>(origin, target);
        } else if (target & board.OccupiedBy<!turn>()) {
          receiver.template move<QUEEN, MOVE_FLAG_CAPTURE>(origin, target);
        } else {
          receiver.template move<QUEEN, MOVE_FLAG_SILENT>(origin, target);
        }
      }
    }
  }
  {
    bitmap_t unpinnedQueens = queens & ~(pinmask.hv | pinmask.d12);
    WHILE_RESET_LSB(unpinnedQueens) {
      const bitmap_t tile = SQUARE_OF(unpinnedQueens);
      const bitmap_t origin = 1ull << tile;
      bitmap_t move = MagicLookup::Queen(tile, board.Occupied()) & movable;
      while (move) {
        const bitmap_t target = popBit(move);
        if (target & board.OccupiedBy<!turn>()) {
          receiver.template move<QUEEN, MOVE_FLAG_CAPTURE>(origin, target);
        } else {
          receiver.template move<QUEEN, MOVE_FLAG_SILENT>(origin, target);
        }
      }
    }
  }
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
  constexpr auto turn = state.turn();
  const bitmap_t pawns = board.Pawns<state.turn()>();
  if ((RelativeEnPassantRank<turn>() & king) &&
      (RelativeEnPassantRank<turn>() & enemyHVSliders) &&
      (RelativeEnPassantRank<turn>() & pawns)) {
    bitmap_t epLeftPawn = pawns & ((g_EnPassantTarget & PawnsNotRight()) >> 1);
    bitmap_t epRightPawn = pawns & ((g_EnPassantTarget & PawnsNotLeft()) << 1);

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
        PawnAttackLeft<!turn>(pawns) & PawnsNotLeft();
    const bitmap_t pawnAttacksRight =
        PawnAttackRight<!turn>(pawns) & PawnsNotRight();
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

template <GameState state, int depth>
force_inline void PawnCheck(bitmap_t enemyKing, bitmap_t target) {
  constexpr bool turn = state.turn();
  bitmap_t pl = PawnAttackLeft<turn>(target & PawnsNotLeft());
  bitmap_t pr = PawnAttackRight<turn>(target & PawnsNotRight());
  if (enemyKing & (pl | pr)) {
    Movestack::g_Checkmask[depth - 1] = target;
  }
}

template <GameState state, int depth>
force_inline void KnightCheck(bitmap_t enemyKing, bitmap_t target) {
  constexpr auto turn = state.turn();
  const bitmap_t enemyKingTile = SQUARE_OF(enemyKing);
  if (MagicLookup::Knight(enemyKingTile) & target) {
    Movestack::g_Checkmask[depth - 1] = target;
  }
}

}  // namespace internal

template <GameState state, figure_type figure, move_flag flag, int depth>
force_inline void PrepareEnumeration(const Board &board,
                                     compiletime_move<figure, flag> move) {
  constexpr auto turn = state.turn();
  if constexpr (figure == KING) return;  // Kings can't give checks.
  if constexpr (figure == PAWN) {
    if constexpr (flag == MOVE_FLAG_DOUBLE_PAWN_PUSH) {
      internal::g_EnPassantTarget = move.m_target;
    }
    internal::PawnCheck<state, depth>(board.King<!turn>(), move.m_target);
  }
  if constexpr (flag == MOVE_FLAG_PROMOTE && figure == KNIGHT) {
    internal::KnightCheck<state, depth>(board.King<!turn>(), move.m_target);
  }
}

template <GameState state, int depth, typename MoveReceiver>
never_inline void Enumerate(const Board &board, MoveReceiver &receiver) {
  checkmask_t checkmask = internal::Movestack::g_Checkmask[depth];
  banmask_t banmask = internal::Movestack::g_KingAttack[depth - 1] =
      internal::Movestack::g_EnemyKingAttack[depth];
  bitmap_t kingAttack =
      internal::refreshStack<state, depth>(board, banmask, checkmask);
  if (checkmask != 0) {
    internal::enumerateMoves<state, MoveReceiver, depth>(
        board, receiver, kingAttack, checkmask, banmask);
  } else {
    bitmap_t king = board.King<state.turn()>();
    WHILE_RESET_LSB(kingAttack) {
      bitmap_t targetTile = SQUARE_OF(kingAttack);
      bitmap_t target = 1ull << targetTile;
      internal::Movestack::g_EnemyKingAttack[depth - 1] =
          MagicLookup::King(targetTile);
      if (target & board.OccupiedBy<!state.turn()>()) {
        receiver.template move<KING, MOVE_FLAG_CAPTURE>(king, target);
      } else {
        receiver.template move<KING, MOVE_FLAG_SILENT>(king, target);
      }
    }
  }
}

}  // namespace Movegen
