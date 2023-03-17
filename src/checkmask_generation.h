#pragma once
#include "Board.h"
#include "BoardState.h"
#include "banmask.h"
#include "checkmask.h"
#include "lookup.h"
#include "pinmask.h"
#include "x86utils.h"

/*
 *  The checkmask is one of tree masks to prune moves effectively.
 *  It is used to prune moves that are illegal because the player is in check.
 *  There the checkmask is defined the following:
 *    (i)  If there is no check all bits of the check masks are set.
 *    (ii) If there exists a check only the bits of tiles are set that can
 *         block or attack the attacking piece are set.
 *         For example if an enemy Knight attacks the King the
 *         bit of the Knights tile should be set.
 *         If a sliding Piece attacks the King the bits of the
 *         attacking piece and all bits of tiles that can block the attack
 *         should be set.
 *  For all valid moves that are not King moves, their target square
 *  should be part of the checkmask
 *
 *  Implementation detail:
 *  The checkmask is a 64bit bitmask where each bit identifies a chess board
 *  tile, where a1 is the first bit and h8 is the last bit. If there is no check
 *  from the enemy player the checkmask has all bits set to 1, otherwise the all
 *  bits are initially set to 0 and some bits are set depending on the piece
 *  checking the king.
 *  (i)   Pawn:
 *          If a pawn checks the king the pawns tile is added to
 *          the checkmask.
 *  (ii)  Knight:
 *          If a knight checks the king the Knights tile is added
 *          to the checkmask
 *  (iii) Sliding Piece:
 *          If a sliding piece checks the king, the path from the enemy
 *          (inclusive) to the king (exclusive) is added to the checkmask.
 *  NOTE: There can only be two check per move.
 */
template <class BoardState state>
inline checkmask_t calculate_checkmask(const Board &board) {
  checkmask_t checkmask = 0;
  // pawn checkmask
  if constexpr (state.turn()) {
    const bitmap_t left_checking_pawns =
        (((board.enemy_pawns_of<state.turn()>() & (~Board::Rank8)) >> 7) &
         board.king_of<state.turn()>())
        << 7;
    checkmask |= left_checking_pawns;
    const bitmap_t right_checking_pawns =
        (((board.enemy_pawns_of<state.turn()>() & (~Board::Rank1)) >> 9) &
         board.king_of<state.turn()>())
        << 9;
    checkmask |= right_checking_pawns;
  } else {
    const bitmap_t left_checking_pawns =
        (((board.enemy_pawns_of<state.turn()>() & (~Board::Rank8)) << 9) &
         board.king_of<state.turn()>()) >>
        9;
    checkmask |= left_checking_pawns;
    const bitmap_t right_checking_pawns =
        (((board.enemy_pawns_of<state.turn()>() & (~Board::Rank1)) << 7) &
         board.king_of<state.turn()>()) >>
        7;
    checkmask |= right_checking_pawns;
  }
  // knight checkmask
  bitmap_t enemyKnights = board.enemy_knights_of<state.turn()>();
  iterate_bits(knight, enemyKnights) {
    if (board.king_of<state.turn()>() &
        KnightLookUpTable::get()[SQUARE_OF(knight)]) {  // ahhh if statements
      checkmask |= knight;
    }
  }
  // Calculate horizontal and vertical sliding checkmask.
  const bitmap_t kingLookupSouth =
      SouthSlidingLookUpTable::get()[SQUARE_OF(board.king_of<state.turn()>())];
  const bitmap_t kingLookupWest =
      WestSlidingLookUpTable::get()[SQUARE_OF(board.king_of<state.turn()>())];
  const bitmap_t kingLookupNorth =
      NorthSlidingLookUpTable::get()[SQUARE_OF(board.king_of<state.turn()>())];
  const bitmap_t kingLookupEast =
      EastSlidingLookUpTable::get()[SQUARE_OF(board.king_of<state.turn()>())];
  bitmap_t enemyRooksAndQueens = board.rooks_and_queens_of<not state.turn()>();
  iterate_bits(hvSliding, enemyRooksAndQueens) {
    const bitmap_t southLookup =
        SouthSlidingLookUpTable::get()[SQUARE_OF(hvSliding)];
    const bitmap_t c0 = (southLookup & ~kingLookupSouth) << 8;
    checkmask |= (southLookup & board.king_of<state.turn()>() &&
                  not((c0 & ~hvSliding) & board.occupied())) *
                 c0;
    const bitmap_t westLookup =
        WestSlidingLookUpTable::get()[SQUARE_OF(hvSliding)];
    const bitmap_t c1 = (westLookup & ~kingLookupWest) << 1;
    checkmask |= (westLookup & board.king_of<state.turn()>() &&
                  not((c1 & ~hvSliding) & board.occupied())) *
                 c1;
    const bitmap_t northLookup =
        NorthSlidingLookUpTable::get()[SQUARE_OF(hvSliding)];
    const bitmap_t c2 = (northLookup & ~kingLookupNorth) >> 8;
    checkmask |= (northLookup & board.king_of<state.turn()>() &&
                  not((c2 & ~hvSliding) & board.occupied())) *
                 c2;
    const bitmap_t eastLookup =
        EastSlidingLookUpTable::get()[SQUARE_OF(hvSliding)];
    const bitmap_t c3 = (eastLookup & ~kingLookupEast) >> 1;
    checkmask |= (eastLookup & board.king_of<state.turn()>() &&
                  not((c3 & ~hvSliding) & board.occupied())) *
                 c3;
  }
  // Calculate diagonal sliding checkmask
  const bitmap_t kingLookupSouthEast =
      SouthEastSlidingLookUpTable::get()[SQUARE_OF(
          board.king_of<state.turn()>())];
  const bitmap_t kingLookupSouthWest =
      SouthWestSlidingLookUpTable::get()[SQUARE_OF(
          board.king_of<state.turn()>())];
  const bitmap_t kingLookupNorthWest =
      NorthWestSlidingLookUpTable::get()[SQUARE_OF(
          board.king_of<state.turn()>())];
  const bitmap_t kingLookupNorthEast =
      NorthEastSlidingLookUpTable::get()[SQUARE_OF(
          board.king_of<state.turn()>())];
  bitmap_t enemyBishopsAndQueens =
      board.bishop_and_queens_of<not state.turn()>();
  iterate_bits(dSliding, enemyBishopsAndQueens) {
    const bitmap_t southEastLookup =
        SouthEastSlidingLookUpTable::get()[SQUARE_OF(dSliding)];
    const bitmap_t c0 = (southEastLookup & ~kingLookupSouthEast) << 7;
    checkmask |= (southEastLookup & board.king_of<state.turn()>() &&
                  not((c0 & ~dSliding) & board.occupied())) *
                 c0;
    const bitmap_t southWestLookup =
        SouthWestSlidingLookUpTable::get()[SQUARE_OF(dSliding)];
    const bitmap_t c1 = (southWestLookup & ~kingLookupSouthWest) << 9;
    checkmask |= (southWestLookup & board.king_of<state.turn()>() &&
                  not((c1 & ~dSliding) & board.occupied())) *
                 c1;
    const bitmap_t northWestLookup =
        NorthWestSlidingLookUpTable::get()[SQUARE_OF(dSliding)];
    const bitmap_t c2 = (northWestLookup & ~kingLookupNorthWest) >> 7;
    checkmask |= (northWestLookup & board.king_of<state.turn()>() &&
                  not((c2 & ~dSliding) & board.occupied())) *
                 c2;
    const bitmap_t northEastLookup =
        NorthEastSlidingLookUpTable::get()[SQUARE_OF(dSliding)];
    const bitmap_t c3 = (northEastLookup & ~kingLookupNorthEast) >> 9;
    checkmask |= (northEastLookup & board.king_of<state.turn()>() &&
                  not((c3 & ~dSliding) & board.occupied())) *
                 c3;
  }
  if (!checkmask) checkmask = -1;
  return checkmask;
}
