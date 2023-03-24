#pragma once

#include "Board.h"
#include "GameState.h"
#include "banmask.h"
#include "checkmask.h"
#include "lookup.h"
#include "pinmask.h"
#include "x86utils.h"

/*
 * The pinmask is the second mask used to prune illegal moves.
 * It prevents pined pieces from moving to illegal squares.
 * It should be noted, that pined pieces are still allowed to move,
 * just only in the direction of the pin.
 * When considering pins we distinguish between 2 types of pins,
 * horizontal or vertical pins and diagonal pins.
 * The pinmask consists of 2 bitmaske one for horizontal and vertical pins
 * and one for diagonal pins.
 * It's possible to combine 2 directions (horizontal and vertical etc.)
 * because we make the assumption that a piece can not be pinned in both
 * directions this is because there can only be one king on the board.
 *
 * The pinmasks are defined by the following rule:
 *      If an enemy sliding piece pins a piece directly to the King,
 *      all squares that the pinned piece can move to should be set in
 *      the pinmask. If a piece is pined horizontally or vertical the
 *      bits should be set in the hv mask, if the piece is diagonally
 *      pinned the bits in diagonal pinmask should be set.
 *      Explicitly this includes all tiles from the king (exclusive) to
 *      the attacking piece (inclusive)
 *
 * The pinmask is applied using the following rules:
 *   (i)  If a pieces square is not set in the pinmask it's moves are not
 *        restricted.
 *   (ii) If a pieces square are set in a pinmask the piece is only allowed
 *        to move inside that pinmask.
 *
 *
 *  Implementation Detail:
 *
 *  Example:
 *  position:
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *  [ ][ ][K][ ][R][ ][r][ ][ ]
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *
 *  pinmask.hv:
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *  [ ][ ][ ][x][x][x][x][ ][ ]
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *  [ ][ ][ ][ ][ ][ ][ ][ ][ ]
 *
 *  The pinmask includes all tiles a pined piece can move towards.
 *  The pinmask has to be separated into Horizontal & Vertical (HV) and
 *  Diagonal (D). If all pins would be stored in one pinmask a piece that in
 * pined horizontally could move to the diagonal pinmask.
 *
 *  Behaviour for all pieces regarding the pinmask:
 *  (i)   Pawn:
 *    A hv-pined pawn can only move vertical, diagonal moves are not allowed.
 *    A d-pined pawn can only move diagonal in the direction of the pin.
 *  (ii)  Knight:
 *    A pined Knight can never move.
 *  (iii) Bishop & Rook:
 *    If a bishop is pined diagonally it can only move on that diagonal.
 *    Rooks are analog with the hv-pinmask.
 *  (iv)  Queen:
 *    If a Queen is pined diagonally it can only move diagonally.
 *    If a Queen is pined hv it can only move horizontal or vertical.
 *
 *  NOTE: There can be upto 8 pins on one board.
 */
template <class GameState state>
inline pinmask_t generate_pinmask(const Board &board) {
  pinmask_t pinmask;
  // Calculate hv pinmask.
  const bitmap_t kingLookupSouth =
      SouthSlidingLookUpTable::get()[SQUARE_OF(board.King<state.turn()>())];
  const bitmap_t kingLookupWest =
      WestSlidingLookUpTable::get()[SQUARE_OF(board.King<state.turn()>())];
  const bitmap_t kingLookupNorth =
      NorthSlidingLookUpTable::get()[SQUARE_OF(board.King<state.turn()>())];
  const bitmap_t kingLookupEast =
      EastSlidingLookUpTable::get()[SQUARE_OF(board.King<state.turn()>())];
  bitmap_t enemyRooksAndQueens =
      board.enemy_rooks_and_queens_of<state.turn()>();
  iterate_bits(hvSliding, enemyRooksAndQueens) {
    const bitmap_t c0 =
        SouthSlidingLookUpTable::get()[SQUARE_OF(hvSliding)] & ~kingLookupSouth;
    pinmask.hv |= (((c0 & board.King<state.turn()>()) &&
                    c0 & board.OccupiedBy<state.turn()>()) &&
                   !(REST_LOWEST_BIT(c0 & board.OccupiedBy<state.turn()>() &
                                     ~board.King<state.turn()>()))) *
                  ((c0 | hvSliding) & ~board.King<state.turn()>());
    const bitmap_t c1 =
        WestSlidingLookUpTable::get()[SQUARE_OF(hvSliding)] & ~kingLookupWest;
    pinmask.hv |= (((c1 & board.King<state.turn()>()) &&
                    (c1 & board.OccupiedBy<state.turn()>())) &&
                   !(REST_LOWEST_BIT(c1 & board.OccupiedBy<state.turn()>() &
                                     ~board.King<state.turn()>()))) *
                  ((c1 | hvSliding) & ~board.King<state.turn()>());
    const bitmap_t c2 =
        NorthSlidingLookUpTable::get()[SQUARE_OF(hvSliding)] & ~kingLookupNorth;
    pinmask.hv |= (((c2 & board.King<state.turn()>()) &&
                    c2 & board.OccupiedBy<state.turn()>()) &&
                   !(REST_LOWEST_BIT(c2 & board.OccupiedBy<state.turn()>() &
                                     ~board.King<state.turn()>()))) *
                  ((c2 | hvSliding) & ~board.King<state.turn()>());
    const bitmap_t c3 =
        EastSlidingLookUpTable::get()[SQUARE_OF(hvSliding)] & ~kingLookupEast;
    pinmask.hv |= (((c3 & board.King<state.turn()>()) &&
                    c3 & board.OccupiedBy<state.turn()>()) &&
                   !(REST_LOWEST_BIT(c3 & board.OccupiedBy<state.turn()>() &
                                     ~board.King<state.turn()>()))) *
                  ((c3 | hvSliding) & ~board.King<state.turn()>());
  }
  const bitmap_t kingLookupSouthEast =
      SouthEastSlidingLookUpTable::get()[SQUARE_OF(board.King<state.turn()>())];
  const bitmap_t kingLookupSouthWest =
      SouthWestSlidingLookUpTable::get()[SQUARE_OF(board.King<state.turn()>())];
  const bitmap_t kingLookupNorthWest =
      NorthWestSlidingLookUpTable::get()[SQUARE_OF(board.King<state.turn()>())];
  const bitmap_t kingLookupNorthEast =
      NorthEastSlidingLookUpTable::get()[SQUARE_OF(board.King<state.turn()>())];
  bitmap_t enemyBishopsAndQueens =
      board.enemy_bishop_and_queens_of<state.turn()>();
  iterate_bits(dSliding, enemyBishopsAndQueens) {
    const bitmap_t c0 =
        SouthEastSlidingLookUpTable::get()[SQUARE_OF(dSliding)] &
        ~kingLookupSouthEast;
    pinmask.d12 |= (((c0 & board.King<state.turn()>()) &&
                   c0 & board.OccupiedBy<state.turn()>()) &&
                  !(_blsr_u64(c0 & board.OccupiedBy<state.turn()>() &
                              ~board.King<state.turn()>()))) *
                 ((c0 | dSliding) & ~board.King<state.turn()>());
    const bitmap_t c1 =
        SouthWestSlidingLookUpTable::get()[SQUARE_OF(dSliding)] &
        ~kingLookupSouthWest;
    pinmask.d12 |= (((c1 & board.King<state.turn()>()) &&
                   c1 & board.OccupiedBy<state.turn()>()) &&
                  !(_blsr_u64(c0 & board.OccupiedBy<state.turn()>() &
                              ~board.King<state.turn()>()))) *
                 ((c1 | dSliding) & ~board.King<state.turn()>());
    const bitmap_t c2 =
        NorthWestSlidingLookUpTable::get()[SQUARE_OF(dSliding)] &
        ~kingLookupNorthWest;
    pinmask.d12 |= (((c2 & board.King<state.turn()>()) &&
                   c2 & board.OccupiedBy<state.turn()>()) &&
                  !(_blsr_u64(c0 & board.OccupiedBy<state.turn()>() &
                              ~board.King<state.turn()>()))) *
                 ((c2 | dSliding) & ~board.King<state.turn()>());
    const bitmap_t c3 =
        NorthEastSlidingLookUpTable::get()[SQUARE_OF(dSliding)] &
        ~kingLookupNorthEast;
    pinmask.d12 |= (((c3 & board.King<state.turn()>()) &&
                   c3 & board.OccupiedBy<state.turn()>()) &&
                  !(_blsr_u64(c0 & board.OccupiedBy<state.turn()>() &
                              ~board.King<state.turn()>()))) *
                 ((c3 | dSliding) & ~board.King<state.turn()>());
  }
  return pinmask;
}
