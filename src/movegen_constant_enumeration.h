//
// Created by karl on 26/03/23.
//
#pragma once

#include <functional>

#include "Board.h"
#include "ChessPosition.h"
#include "GameState.h"
#include "MagicLookup.h"
#include "banmask.h"
#include "bitmap.h"
#include "checkmask.h"
#include "move.h"
#include "movegen_movestack.h"
#include "movegen_shift.h"
#include "movegen_util.h"
#include "pinmask.h"
#include "x86utils.h"

namespace movegen::constant_enumeration {

template <class GameState state, typename MoveReceiver, int depth>
inline void _enumeration(const Board& board, MoveReceiver& receiver,
                         bitmap_t kingAttack, checkmask_t checkmask,
                         banmask_t banmask) {
  constexpr color_t turn = state.turn();
  const bool noCheck = (checkmask == 0xFFFFFFFFFFFFFFFFull);

  const pinmask_t pinmask(Movestack::g_RookPin, Movestack::g_BishopPin);
  const bitmap_t movable = board.EnemyOrEmpty<turn>() & checkmask;
  const bitmap_t epTarget = Movestack::g_EnPassantTarget;

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
      if (noCheck && util::canCastleLeft<state>(banmask, board.Occupied(),
                                                board.Rooks<turn>(), board)) {
        Movestack::g_EnemyKingAttack[depth - 1] =
            MagicLookup::King(SQUARE_OF(king << 2));
        receiver.template move<KING, MOVE_FLAG_LONG_CASTLE>(turn, turn << 2);
      }
    }
    if constexpr (state.canCastleShort()) {
      if (noCheck && util::canCastleRight<state>(banmask, board.Occupied(),
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
        pawnsD12 &
        shift::PawnUndoAttackLeft<turn>(board.OccupiedBy<!turn>() &
                                        shift::PawnsNotRight() & checkmask);
    bitmap_t rightAttackingPawns =
        pawnsD12 &
        shift::PawnUndoAttackRight<turn>(board.OccupiedBy<!turn>() &
                                         shift::PawnsNotLeft() & checkmask);

    bitmap_t pushablePawns =
        pawnsHV & shift::PawnBackward<turn>(board.NotOccupied());

    bitmap_t doublePushablePawns =
        pushablePawns & shift::RelativeFirstRank<turn>() &
        shift::PawnBackward2<turn>(board.NotOccupied() & checkmask);
    pushablePawns &= shift::PawnBackward<turn>(checkmask);

    shift::PawnPruneLeft<turn>(leftAttackingPawns, pinmask.d12);
    shift::PawnPruneRight<turn>(rightAttackingPawns, pinmask.d12);
    shift::PawnPrunePush<turn>(pushablePawns, pinmask.hv);
    shift::PawnPruneDoublePush<turn>(doublePushablePawns, pinmask.hv);

    if constexpr (state.hasEnPassant()) {
      bitmap_t epLeftPawn =
          pawnsD12 & shift::PawnsNotLeft() & ((epTarget & checkmask) >> 1);
      bitmap_t epRightPawn =
          pawnsD12 & shift::PawnsNotRight() & ((epTarget & checkmask) << 1);

      if (epLeftPawn | epRightPawn) {
        shift::PawnPruneLeftEP<turn>(epLeftPawn, pinmask.d12);
        shift::PawnPruneRightEP<turn>(epRightPawn, pinmask.d12);
        if (epLeftPawn) {
          receiver.template move<PAWN, MOVE_FLAG_EN_PASSANT>(
              epLeftPawn, shift::PawnAttackLeft<turn>(epLeftPawn));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        }
        if (epRightPawn) {
          receiver.template move<PAWN, MOVE_FLAG_EN_PASSANT>(
              epRightPawn, shift::PawnAttackRight<turn>(epRightPawn));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        }
      }
    }
    if ((leftAttackingPawns | rightAttackingPawns | pushablePawns) &
        shift::RelativeSecondLastRank<turn>()) {
      bitmap_t promoteLeft =
          leftAttackingPawns & shift::RelativeSecondLastRank<turn>();
      bitmap_t promoteRight =
          rightAttackingPawns & shift::RelativeSecondLastRank<turn>();
      bitmap_t promotePush =
          pushablePawns & shift::RelativeSecondLastRank<turn>();

      bitmap_t noPromoteLeft =
          leftAttackingPawns & shift::RelativeSecondLastRank<turn>();
      bitmap_t noPromoteRight =
          rightAttackingPawns & shift::RelativeSecondLastRank<turn>();
      bitmap_t noPromotePush =
          pushablePawns & shift::RelativeSecondLastRank<turn>();

      while (promoteLeft) {
        const bitmap_t pos = popBit(promoteLeft);
        receiver.template move<KNIGHT, MOVE_FLAG_PROMOTE>(
            pos, shift::PawnAttackLeft<turn>(pos));
        Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        receiver.template move<BISHOP, MOVE_FLAG_PROMOTE>(
            pos, shift::PawnAttackLeft<turn>(pos));
        receiver.template move<ROOK, MOVE_FLAG_PROMOTE>(
            pos, shift::PawnAttackLeft<turn>(pos));
        receiver.template move<QUEEN, MOVE_FLAG_PROMOTE>(
            pos, shift::PawnAttackLeft<turn>(pos));
      }
      while (promoteRight) {
        const bitmap_t pos = popBit(promoteRight);
        receiver.template move<KNIGHT, MOVE_FLAG_PROMOTE>(
            pos, shift::PawnAttackRight<turn>(pos));
        Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        receiver.template move<BISHOP, MOVE_FLAG_PROMOTE>(
            pos, shift::PawnAttackRight<turn>(pos));
        receiver.template move<ROOK, MOVE_FLAG_PROMOTE>(
            pos, shift::PawnAttackRight<turn>(pos));
        receiver.template move<QUEEN, MOVE_FLAG_PROMOTE>(
            pos, shift::PawnAttackRight<turn>(pos));
      }
      while (promotePush) {
        const bitmap_t pos = popBit(promotePush);
        receiver.template move<KNIGHT, MOVE_FLAG_PROMOTE>(
            pos, shift::PawnForward<turn>(pos));
        Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        receiver.template move<BISHOP, MOVE_FLAG_PROMOTE>(
            pos, shift::PawnForward<turn>(pos));
        receiver.template move<ROOK, MOVE_FLAG_PROMOTE>(
            pos, shift::PawnForward<turn>(pos));
        receiver.template move<QUEEN, MOVE_FLAG_PROMOTE>(
            pos, shift::PawnForward<turn>(pos));
      }
      while (noPromoteLeft) {
        const bitmap_t pos = popBit(noPromoteLeft);
        receiver.template move<PAWN, MOVE_FLAG_CAPTURE>(
            pos, shift::PawnAttackLeft<turn>(pos));
        Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
      }
      while (noPromoteRight) {
        const bitmap_t pos = popBit(noPromoteRight);
        receiver.template move<PAWN, MOVE_FLAG_CAPTURE>(
            pos, shift::PawnAttackRight<turn>(pos));
        Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
      }
      while (noPromotePush) {
        const bitmap_t pos = popBit(noPromotePush);
        receiver.template move<PAWN, MOVE_FLAG_SILENT>(
            pos, shift::PawnForward<turn>(pos));
        Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
      }
      while (doublePushablePawns) {
        const bitmap_t pos = popBit(doublePushablePawns);
        receiver.template move<PAWN, MOVE_FLAG_DOUBLE_PAWN_PUSH>(
            pos, shift::PawnForward2<turn>(pos));
        Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
      }

    } else {
      while (leftAttackingPawns) {
        const bitmap_t pos = popBit(leftAttackingPawns);
        receiver.template move<PAWN, MOVE_FLAG_CAPTURE>(
            pos, shift::PawnAttackLeft<turn>(pos));
        Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
      }
      while (rightAttackingPawns) {
        const bitmap_t pos = popBit(rightAttackingPawns);
        receiver.template move<PAWN, MOVE_FLAG_CAPTURE>(
            pos, shift::PawnAttackRight<turn>(pos));
        Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
      }
      while (pushablePawns) {
        const bitmap_t pos = popBit(pushablePawns);
        receiver.template move<PAWN, MOVE_FLAG_SILENT>(
            pos, shift::PawnForward<turn>(pos));
        Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
      }
      while (doublePushablePawns) {
        const bitmap_t pos = popBit(doublePushablePawns);
        receiver.template move<PAWN, MOVE_FLAG_DOUBLE_PAWN_PUSH>(
            pos, shift::PawnForward2<turn>(pos));
        Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
      }
    }
  }
  // Knightmoves
  {
    bitmap_t knights = board.Knights<turn>() & ~(pinmask.hv | pinmask.d12);
    WHILE_RESET_LSB(knights) {
      const bitmap_t tile = SQUARE_OF(knights);
      bitmap_t origin = 1ull << tile;
      bitmap_t moves = MagicLookup::King(tile) & movable;
      while (moves) {
        const bitmap_t target = popBit(moves);
        if (target & board.OccupiedBy<!turn>()) {
          receiver.template move<KNIGHT, MOVE_FLAG_CAPTURE>(origin, target);
        } else {
          receiver.template move<KNIGHT, MOVE_FLAG_SILENT>(origin, target);
        }
        Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
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
        if (origin & shift::RelativeLeftRookMask<turn>()) {
          receiver.template move<QUEEN, MOVE_FLAG_LEFT_ROOK>(origin, target);
        } else if (origin & shift::RelativeRightRookMask<turn>()) {
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

template <GameState state, int depth>
force_inline void _pawnCheck(bitmap_t enemyKing, bitmap_t target) {
  constexpr bool turn = state.turn();
  bitmap_t pl = shift::PawnAttackLeft<turn>(target & shift::PawnsNotLeft());
  bitmap_t pr = shift::PawnAttackRight<turn>(target & shift::PawnsNotRight());
  if (enemyKing & (pl | pr)) {
    Movestack::g_Checkmask[depth - 1] = target;
  }
}

template <GameState state, int depth>
force_inline void _knightCheck(bitmap_t enemyKing, bitmap_t target) {
  constexpr auto turn = state.turn();
  const bitmap_t enemyKingTile = SQUARE_OF(enemyKing);
  if (MagicLookup::Knight(enemyKingTile) & target) {
    Movestack::g_Checkmask[depth - 1] = target;
  }
}

template <GameState state, figure figure, move_flag flag, int depth>
force_inline void prepare(const Board& board,
                          const compiletime_move<figure, flag>& move) {
  constexpr auto turn = state.turn();
  if constexpr (figure == KING) return;  // Kings can't give checks.
  if constexpr (figure == PAWN) {
    if constexpr (flag == MOVE_FLAG_DOUBLE_PAWN_PUSH) {
      Movestack::g_EnPassantTarget = move.m_target;
    }
    _pawnCheck<state, depth>(board.King<!turn>(), move.m_target);
  }
  if constexpr (flag == MOVE_FLAG_PROMOTE && figure == KNIGHT) {
    _knightCheck<state, depth>(board.King<!turn>(), move.m_target);
  }
}

template <GameState state, int depth, typename MoveReceiver>
never_inline void recursive_call(const Board& board, MoveReceiver& receiver) {
  checkmask_t checkmask = Movestack::g_Checkmask[depth];
  banmask_t banmask = Movestack::g_KingAttack[depth - 1] =
      Movestack::g_EnemyKingAttack[depth];
  bitmap_t kingAttack =
      Movestack::refreshStack<state, depth>(board, banmask, checkmask);
  if (checkmask != 0) {
    constexpr color_t turn = state.turn();
    const bool noCheck = (checkmask == 0xFFFFFFFFFFFFFFFFull);

    const pinmask_t pinmask(Movestack::g_RookPin, Movestack::g_BishopPin);
    const bitmap_t movable = board.EnemyOrEmpty<turn>() & checkmask;
    const bitmap_t epTarget = Movestack::g_EnPassantTarget;

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
        if (noCheck && util::canCastleLeft<state>(banmask, board.Occupied(),
                                                  board.Rooks<turn>(), board)) {
          Movestack::g_EnemyKingAttack[depth - 1] =
              MagicLookup::King(SQUARE_OF(king << 2));
          receiver.template move<KING, MOVE_FLAG_LONG_CASTLE>(turn, turn << 2);
        }
      }
      if constexpr (state.canCastleShort()) {
        if (noCheck && util::canCastleRight<state>(banmask, board.Occupied(),
                                                   board.Occupied(), board)) {
          Movestack::g_EnemyKingAttack[depth - 1] =
              MagicLookup::King(king >> 2);
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
          pawnsD12 &
          shift::PawnUndoAttackLeft<turn>(board.OccupiedBy<!turn>() &
                                          shift::PawnsNotRight() & checkmask);
      bitmap_t rightAttackingPawns =
          pawnsD12 &
          shift::PawnUndoAttackRight<turn>(board.OccupiedBy<!turn>() &
                                           shift::PawnsNotLeft() & checkmask);

      bitmap_t pushablePawns =
          pawnsHV & shift::PawnBackward<turn>(board.NotOccupied());

      bitmap_t doublePushablePawns =
          pushablePawns & shift::RelativeFirstRank<turn>() &
          shift::PawnBackward2<turn>(board.NotOccupied() & checkmask);
      pushablePawns &= shift::PawnBackward<turn>(checkmask);

      shift::PawnPruneLeft<turn>(leftAttackingPawns, pinmask.d12);
      shift::PawnPruneRight<turn>(rightAttackingPawns, pinmask.d12);
      shift::PawnPrunePush<turn>(pushablePawns, pinmask.hv);
      shift::PawnPruneDoublePush<turn>(doublePushablePawns, pinmask.hv);

      if constexpr (state.hasEnPassant()) {
        bitmap_t epLeftPawn =
            pawnsD12 & shift::PawnsNotLeft() & ((epTarget & checkmask) >> 1);
        bitmap_t epRightPawn =
            pawnsD12 & shift::PawnsNotRight() & ((epTarget & checkmask) << 1);

        if (epLeftPawn | epRightPawn) {
          shift::PawnPruneLeftEP<turn>(epLeftPawn, pinmask.d12);
          shift::PawnPruneRightEP<turn>(epRightPawn, pinmask.d12);
          if (epLeftPawn) {
            receiver.template move<PAWN, MOVE_FLAG_EN_PASSANT>(
                epLeftPawn, shift::PawnAttackLeft<turn>(epLeftPawn));
            Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
          }
          if (epRightPawn) {
            receiver.template move<PAWN, MOVE_FLAG_EN_PASSANT>(
                epRightPawn, shift::PawnAttackRight<turn>(epRightPawn));
            Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
          }
        }
      }
      if ((leftAttackingPawns | rightAttackingPawns | pushablePawns) &
          shift::RelativeSecondLastRank<turn>()) {
        bitmap_t promoteLeft =
            leftAttackingPawns & shift::RelativeSecondLastRank<turn>();
        bitmap_t promoteRight =
            rightAttackingPawns & shift::RelativeSecondLastRank<turn>();
        bitmap_t promotePush =
            pushablePawns & shift::RelativeSecondLastRank<turn>();

        bitmap_t noPromoteLeft =
            leftAttackingPawns & shift::RelativeSecondLastRank<turn>();
        bitmap_t noPromoteRight =
            rightAttackingPawns & shift::RelativeSecondLastRank<turn>();
        bitmap_t noPromotePush =
            pushablePawns & shift::RelativeSecondLastRank<turn>();

        while (promoteLeft) {
          const bitmap_t pos = popBit(promoteLeft);
          receiver.template move<KNIGHT, MOVE_FLAG_PROMOTE>(
              pos, shift::PawnAttackLeft<turn>(pos));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
          receiver.template move<BISHOP, MOVE_FLAG_PROMOTE>(
              pos, shift::PawnAttackLeft<turn>(pos));
          receiver.template move<ROOK, MOVE_FLAG_PROMOTE>(
              pos, shift::PawnAttackLeft<turn>(pos));
          receiver.template move<QUEEN, MOVE_FLAG_PROMOTE>(
              pos, shift::PawnAttackLeft<turn>(pos));
        }
        while (promoteRight) {
          const bitmap_t pos = popBit(promoteRight);
          receiver.template move<KNIGHT, MOVE_FLAG_PROMOTE>(
              pos, shift::PawnAttackRight<turn>(pos));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
          receiver.template move<BISHOP, MOVE_FLAG_PROMOTE>(
              pos, shift::PawnAttackRight<turn>(pos));
          receiver.template move<ROOK, MOVE_FLAG_PROMOTE>(
              pos, shift::PawnAttackRight<turn>(pos));
          receiver.template move<QUEEN, MOVE_FLAG_PROMOTE>(
              pos, shift::PawnAttackRight<turn>(pos));
        }
        while (promotePush) {
          const bitmap_t pos = popBit(promotePush);
          receiver.template move<KNIGHT, MOVE_FLAG_PROMOTE>(
              pos, shift::PawnForward<turn>(pos));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
          receiver.template move<BISHOP, MOVE_FLAG_PROMOTE>(
              pos, shift::PawnForward<turn>(pos));
          receiver.template move<ROOK, MOVE_FLAG_PROMOTE>(
              pos, shift::PawnForward<turn>(pos));
          receiver.template move<QUEEN, MOVE_FLAG_PROMOTE>(
              pos, shift::PawnForward<turn>(pos));
        }
        while (noPromoteLeft) {
          const bitmap_t pos = popBit(noPromoteLeft);
          receiver.template move<PAWN, MOVE_FLAG_CAPTURE>(
              pos, shift::PawnAttackLeft<turn>(pos));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        }
        while (noPromoteRight) {
          const bitmap_t pos = popBit(noPromoteRight);
          receiver.template move<PAWN, MOVE_FLAG_CAPTURE>(
              pos, shift::PawnAttackRight<turn>(pos));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        }
        while (noPromotePush) {
          const bitmap_t pos = popBit(noPromotePush);
          receiver.template move<PAWN, MOVE_FLAG_SILENT>(
              pos, shift::PawnForward<turn>(pos));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        }
        while (doublePushablePawns) {
          const bitmap_t pos = popBit(doublePushablePawns);
          receiver.template move<PAWN, MOVE_FLAG_DOUBLE_PAWN_PUSH>(
              pos, shift::PawnForward2<turn>(pos));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        }

      } else {
        while (leftAttackingPawns) {
          const bitmap_t pos = popBit(leftAttackingPawns);
          receiver.template move<PAWN, MOVE_FLAG_CAPTURE>(
              pos, shift::PawnAttackLeft<turn>(pos));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        }
        while (rightAttackingPawns) {
          const bitmap_t pos = popBit(rightAttackingPawns);
          receiver.template move<PAWN, MOVE_FLAG_CAPTURE>(
              pos, shift::PawnAttackRight<turn>(pos));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        }
        while (pushablePawns) {
          const bitmap_t pos = popBit(pushablePawns);
          receiver.template move<PAWN, MOVE_FLAG_SILENT>(
              pos, shift::PawnForward<turn>(pos));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        }
        while (doublePushablePawns) {
          const bitmap_t pos = popBit(doublePushablePawns);
          receiver.template move<PAWN, MOVE_FLAG_DOUBLE_PAWN_PUSH>(
              pos, shift::PawnForward2<turn>(pos));
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
        }
      }
    }
    // Knightmoves
    {
      bitmap_t knights = board.Knights<turn>() & ~(pinmask.hv | pinmask.d12);
      WHILE_RESET_LSB(knights) {
        const bitmap_t tile = SQUARE_OF(knights);
        bitmap_t origin = 1ull << tile;
        bitmap_t moves = MagicLookup::Knight(tile) & movable;
        while (moves) {
          const bitmap_t target = popBit(moves);
          if (target & board.OccupiedBy<!turn>()) {
            receiver.template move<KNIGHT, MOVE_FLAG_CAPTURE>(origin, target);
          } else {
            receiver.template move<KNIGHT, MOVE_FLAG_SILENT>(origin, target);
          }
          Movestack::g_Checkmask[depth - 1] = 0xFFFFFFFFFFFFFFFFull;
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
          if (origin & shift::RelativeLeftRookMask<turn>()) {
            receiver.template move<QUEEN, MOVE_FLAG_LEFT_ROOK>(origin, target);
          } else if (origin & shift::RelativeRightRookMask<turn>()) {
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
  } else {
    bitmap_t king = board.King<state.turn()>();
    WHILE_RESET_LSB(kingAttack) {
      bitmap_t targetTile = SQUARE_OF(kingAttack);
      bitmap_t target = 1ull << targetTile;
      Movestack::g_EnemyKingAttack[depth - 1] = MagicLookup::King(targetTile);
      if (target & board.OccupiedBy<!state.turn()>()) {
        receiver.template move<KING, MOVE_FLAG_CAPTURE>(king, target);
      } else {
        receiver.template move<KING, MOVE_FLAG_SILENT>(king, target);
      }
    }
  }
}

template <GameState state, int depth, typename MoveReceiver>
force_inline void entry_call(const Board& board, bitmap_t epTarget,
                             MoveReceiver& receiver) {
  movegen::Movestack::init<state, depth>(board, epTarget);
  recursive_call<state, depth>(board, receiver);
}

template <typename MoveReceiver>
never_inline void runtime_entry_call(const Board& board, const GameState& state,
                                     bitmap_t epTarget,
                                     MoveReceiver& receiver) {
  if (state.turn()) {
    if (state.hasEnPassant()) {
      if (state.whiteHasLongCastle()) {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, true, true, true, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, true, true, true, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, true, true, true, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, true, true, true, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, true, true, false, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, true, true, false, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, true, true, false, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, true, true, false, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        }
      } else {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, true, false, true, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, true, false, true, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, true, false, true, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, true, false, true, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, true, false, false, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, true, false, false, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, true, false, false, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, true, false, false, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        }
      }
    } else {
      if (state.whiteHasLongCastle()) {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, false, true, true, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, false, true, true, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, false, true, true, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, false, true, true, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, false, true, false, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, false, true, false, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, false, true, false, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, false, true, false, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        }
      } else {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, false, false, true, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, false, false, true, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, false, false, true, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, false, false, true, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, false, false, false, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, false, false, false, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(true, false, false, false, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(true, false, false, false, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        }
      }
    }
  } else {
    if (state.hasEnPassant()) {
      if (state.whiteHasLongCastle()) {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, true, true, true, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, true, true, true, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, true, true, true, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, true, true, true, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, true, true, false, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, true, true, false, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, true, true, false, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, true, true, false, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        }
      } else {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, true, false, true, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, true, false, true, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, true, false, true, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, true, false, true, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, true, false, false, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, true, false, false, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, true, false, false, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, true, false, false, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        }
      }
    } else {
      if (state.whiteHasLongCastle()) {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, false, true, true, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, false, true, true, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, false, true, true, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, false, true, true, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, false, true, false, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, false, true, false, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, false, true, false, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, false, true, false, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        }
      } else {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, false, false, true, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, false, false, true, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, false, false, true, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, false, false, true, false, false), 0>(
                  board, epTarget, receiver);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, false, false, false, true, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, false, false, false, true, false), 0>(
                  board, epTarget, receiver);
            }
          } else {
            if (state.blackHasShortCastle()) {
              entry_call<GameState(false, false, false, false, false, true), 0>(
                  board, epTarget, receiver);
            } else {
              entry_call<GameState(false, false, false, false, false, false),
                         0>(board, epTarget, receiver);
            }
          }
        }
      }
    }
  }
}

template <typename MoveReceiver>
force_inline void runtime_entry_call(const ChessPosition& position,
                                     MoveReceiver& receiver) {
  runtime_entry_call(position.board(), position.state(), position.epTarget(),
                     receiver);
}

}  // namespace movegen::constant_enumeration
