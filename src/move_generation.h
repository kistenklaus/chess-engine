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

template <class GameState state, typename MoveReceiver>
inline void enumerateMoves(const Board &board, MoveReceiver &receiver,
                           checkmask_t checkmask, pinmask_t pinmask,
                           banmask_t banmask) {
  constexpr color_t turn = state.turn();

  const bitmap_t movable = board.EnemyOrEmpty<turn>() & checkmask;
  // generate_king_moves<state>(board, receiver, checkmask, banmask);
  // Kingmoves
  {
    bitmap_t kingMap = board.King<turn>();
    bitmap_t kingMoves =
        KingLookUpTable::get()[SQUARE_OF(kingMap)] & movable & (~banmask);
    WHILE_RESET_LSB(kingMoves) {
      const bitmap_t targetTile = SQUARE_OF(kingMoves);
      receiver.template move<KING, MOVE_COMPILETIME_FLAG_SILENT>(
          kingMap, 1ull << targetTile, MOVE_FLAG_NONE);
    }
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

    while (leftAttackingPawns) {
      const bitmap_t pos = popBit(leftAttackingPawns);
      receiver.template move<PAWN, MOVE_COMPILETIME_FLAG_SILENT>(
          pos, PawnAttackLeft<turn>(pos), MOVE_FLAG_NONE);
    }
    while (rightAttackingPawns) {
      const bitmap_t pos = popBit(rightAttackingPawns);
      receiver.template move<PAWN, MOVE_COMPILETIME_FLAG_SILENT>(
          pos, PawnAttackRight<turn>(pos), MOVE_FLAG_NONE);
    }
    while (pushablePawns) {
      const bitmap_t pos = popBit(pushablePawns);
      receiver.template move<PAWN, MOVE_COMPILETIME_FLAG_SILENT>(
          pos, PawnForward<turn>(pos), MOVE_FLAG_NONE);
    }
    while (doublePushablePawns) {
      const bitmap_t pos = popBit(doublePushablePawns);
      receiver.template move<PAWN, MOVE_COMPILETIME_FLAG_SILENT>(
          pos, PawnForward2<turn>(pos), MOVE_FLAG_NONE);
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
        receiver.template move<KNIGHT, MOVE_COMPILETIME_FLAG_SILENT>(
            origin, target, MOVE_FLAG_NONE);
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
          receiver.template move<QUEEN, MOVE_COMPILETIME_FLAG_SILENT>(
              origin, target, MOVE_FLAG_NONE);
        }
      } else {
        while (move) {
          const bitmap_t target = popBit(move);
          receiver.template move<BISHOP, MOVE_COMPILETIME_FLAG_SILENT>(
              origin, target, MOVE_FLAG_NONE);
        }
      }
    }
    WHILE_RESET_LSB(bishopsUnpinned) {
      const bitmap_t tile = SQUARE_OF(bishopsUnpinned);
      bitmap_t origin = 1ull << tile;
      bitmap_t move = MagicLookup::Bishop(tile, board.Occupied()) & movable;
      while (move) {
        const bitmap_t target = popBit(move);
        receiver.template move<BISHOP, MOVE_COMPILETIME_FLAG_SILENT>(
            origin, target, MOVE_FLAG_NONE);
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
          receiver.template move<QUEEN, MOVE_COMPILETIME_FLAG_SILENT>(
              origin, target, MOVE_FLAG_NONE);
        }
      } else {
        const bitmap_t target = popBit(move);
        receiver.template move<ROOK, MOVE_COMPILETIME_FLAG_SILENT>(
            origin, target, MOVE_FLAG_NONE);
      }
    }
    WHILE_RESET_LSB(rooksUnpinned) {
      const bitmap_t tile = SQUARE_OF(rooksUnpinned);
      bitmap_t move = MagicLookup::Rook(tile, board.Occupied()) & movable;
      bitmap_t origin = 1ull << tile;
      while (move) {
        const bitmap_t target = popBit(move);
        receiver.template move<ROOK, MOVE_COMPILETIME_FLAG_SILENT>(
            origin, target, MOVE_FLAG_NONE);
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
        receiver.template move<QUEEN, MOVE_COMPILETIME_FLAG_SILENT>(
            origin, target, MOVE_FLAG_NONE);
      }
    }
  }
}

}  // namespace internal

template<GameState state, typename MoveReceiver>
void Enumerate(Board& board){
  bitmap_t checkmask;

}

}  // namespace Movegen
