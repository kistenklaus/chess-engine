#pragma once
#include "Board.h"
#include "GameState.h"
#include "banmask.h"
#include "checkmask.h"
#include "lookup.h"
#include "pinmask.h"
#include "x86utils.h"

/*
 * The banmask is the last of the masks to prune moves.
 * It is used to prune moves where the King would move into check.
 * This mask is without a doubt the simplest of the 3 masks.
 * It simply contains all tiles the enemy player is attacking.
 * It should be noted however that this is not the same as all possible moves
 * the other player can make. For example a pinned Rook can still block the
 * King from moving even if it is not possible for the Rook to move to the
 * square to capture the King.
 */
template <class GameState state>
inline banmask_t generate_banmask(const Board &board) {
  // Calculate king banmask.
  // NOTE: will probably create a branch in asm. Could be removed if, with the
  // tradeoff that only valid positions would be allowed.
  banmask_t banmask = 0;
  if (board.King<not state.turn()>()) {
    bitmap_t king_attacks =
        KingLookUpTable::get()[SQUARE_OF(board.King<not state.turn()>())] &
        ~board.Occupied();
    banmask |= king_attacks;
  }

  // Calculate pawn attacks.
  bitmap_t pawns = board.enemy_pawns_of<state.turn()>();
  if constexpr (state.turn()) {
    // pawn attack left.
    const bitmap_t left_attacking_pawn(
        (pawns & ~(Board::File1 | Board::Rank1)) >> 9);
    banmask |= left_attacking_pawn;
    // pawn attack right.
    const bitmap_t right_attacking_pawn(
        (pawns & ~(Board::File1 | Board::Rank1)) >> 7);
    banmask |= right_attacking_pawn;

  } else {
    // pawns attack left.
    const bitmap_t left_attacking_pawn =
        ((pawns & ~(Board::File1 | Board::Rank8)) << 7);
    banmask |= left_attacking_pawn;
    // pawns attack right.
    const bitmap_t right_attacking_pawn =
        ((pawns & ~(Board::File8 | Board::Rank8)) << 9);
    banmask |= right_attacking_pawn;
  }

  // Calculate knight attacks.

  bitmap_t knights = board.enemy_knights_of<state.turn()>();
  iterate_bits(knight, knights) {
    bitmap_t lookup =
        KnightLookUpTable::get()[SQUARE_OF(knight)] & board.NotOccupied();
    banmask |= lookup;
  }

  // Calculate hv sliding piece attacks.
  bitmap_t rooksAndQueens = board.rooks_and_queens_of<not state.turn()>();
  iterate_bits(hvSliding, rooksAndQueens) {
    // hvSliding to the left.
    const bitmap_t lookup1 =
        WestSlidingLookUpTable::get()[SQUARE_OF(hvSliding)];
    const bitmap_t hit1 =
        (lookup1 & board.Occupied()) &
        (((bitmap_t)(-1)) << (-_lzcnt_u64(lookup1 & board.Occupied()) - 1));
    banmask |=
        lookup1 & ~((!!hit1) * WestSlidingLookUpTable::get()[SQUARE_OF(hit1)]);

    const bitmap_t lookup2 =
        SouthSlidingLookUpTable::get()[SQUARE_OF(hvSliding)];
    const bitmap_t hit2 = (lookup2 & board.Occupied()) &
                          (((bitmap_t)-1))
                              << (-_lzcnt_u64(lookup2 & board.Occupied()) - 1);
    banmask |=
        lookup2 & ~((!!hit2) * SouthSlidingLookUpTable::get()[SQUARE_OF(hit2)]);

    const bitmap_t lookup3 =
        NorthSlidingLookUpTable::get()[SQUARE_OF(hvSliding)];
    const bitmap_t hit3 = _blsi_u64(lookup3 & board.Occupied());
    banmask |=
        lookup3 & ~((!!hit3) * NorthSlidingLookUpTable::get()[SQUARE_OF(hit3)]);

    const bitmap_t lookup4 =
        EastSlidingLookUpTable::get()[SQUARE_OF(hvSliding)];
    const bitmap_t hit4 = _blsi_u64(lookup4 & board.Occupied());
    banmask |=
        lookup4 & ~((!!hit4) * EastSlidingLookUpTable::get()[SQUARE_OF(hit4)]);
  }

  // Calculate diagonal sliding piece attacks.
  bitmap_t bishopsAndQueens = board.bishop_and_queens_of<not state.turn()>();
  iterate_bits(dSliding, bishopsAndQueens) {
    const bitmap_t lookup1 =
        SouthEastSlidingLookUpTable::get()[SQUARE_OF(dSliding)];
    const bitmap_t hit1 =
        (lookup1 & board.Occupied()) &
        (((bitmap_t)-1) << (-_lzcnt_u64(lookup1 & board.Occupied()) - 1));
    banmask |= lookup1 & ~((!!hit1) *
                           SouthEastSlidingLookUpTable::get()[SQUARE_OF(hit1)]);

    const bitmap_t lookup2 =
        SouthWestSlidingLookUpTable::get()[SQUARE_OF(dSliding)];
    const bitmap_t hit2 =
        (lookup2 & board.Occupied()) &
        (((bitmap_t)-1) << (-_lzcnt_u64(lookup2 & board.Occupied()) - 1));
    banmask |= lookup2 & ~((!!hit2) *
                           SouthWestSlidingLookUpTable::get()[SQUARE_OF(hit2)]);

    const bitmap_t lookup3 =
        NorthWestSlidingLookUpTable::get()[SQUARE_OF(dSliding)];
    const bitmap_t hit3 = _blsi_u64(lookup3 & board.Occupied());
    banmask |= lookup3 & ~((!!hit3) *
                           NorthWestSlidingLookUpTable::get()[SQUARE_OF(hit3)]);

    const bitmap_t lookup4 =
        NorthEastSlidingLookUpTable::get()[SQUARE_OF(dSliding)];
    const bitmap_t hit4 = _blsi_u64(lookup4 & board.Occupied());
    banmask |= lookup4 & ~((!!hit4) *
                           NorthEastSlidingLookUpTable::get()[SQUARE_OF(hit4)]);
  }

  return banmask;
}
