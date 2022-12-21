#pragma once

#include <x86intrin.h>
#include "Board.h"
#include "BoardState.h"
#include "NorthEastSlidingLookUpTable.hpp"
#include "SouthSlidingLookUpTable.hpp"
#include "bitmap.h"
#include "build_config.h"
#include "lookup.h"
#include "move.h"
#include "x86utils.h"

const bitmap_t rank7 = 0xFF000000000000;
const bitmap_t rank8 = 0xFF00000000000000;
const bitmap_t rank1 = 0xFF;
const bitmap_t rank2 = 0xFF00;
const bitmap_t file8 = 0x8080808080808080;
const bitmap_t file7 = file8 >> 1;
const bitmap_t file2 = file7 >> 5;
const bitmap_t file1 = file2 >> 1;

struct pinmask_t {
  bitmap_t hv;
  bitmap_t d;
};

typedef bitmap_t checkmask_t;

typedef bitmap_t banmask_t;

template <class BoardState state>
inline void generate_moves(const Board &board,
                           void (*move_callback)(const move_t move)) {
  // calculate masks
  const checkmask_t checkmask = calculate_checkmask<state>(board);
  const pinmask_t pinmask = calculate_pinmask<state>(board);
  const banmask_t banmask = calculate_banmask<state>(board);
  if constexpr (LOGGING && MASK_LOGGING) {
    std::cout << "====CHECKMASK===" << std::endl;
    std::cout << bitmap_to_bitboard_string(checkmask) << std::endl;
    std::cout << "====PINMASK-HV==" << std::endl;
    std::cout << bitmap_to_bitboard_string(pinmask.hv) << std::endl;
    std::cout << "====PINMASK-D===" << std::endl;
    std::cout << bitmap_to_bitboard_string(pinmask.d) << std::endl;
    std::cout << "====BANMASK=====" << std::endl;
    std::cout << bitmap_to_bitboard_string(banmask) << std::endl;

  }
  // generate moves.
  generate_king_moves<state>(board, move_callback, checkmask, banmask);
  generate_pawn_moves<state>(board, move_callback, checkmask, pinmask);
  generate_knight_moves<state>(board, move_callback, checkmask, pinmask);
  generate_sliding_moves<state>(board, move_callback, checkmask, pinmask);
}

/**
 *  Calculates the checkmask.
 *
 *  The checkmask is a 64bit bitmask where each bit identifies a chess board
 * tile, where a1 is the first bit and h8 is the last bit. If there is no check
 * from the enemy player the checkmask has all bits set to 1, otherwise the all
 * bits are initaly set to 0 and some bits are set depending on the piece
 *  checking the king.
 *  (i)   Pawn:
 *     If a pawn checks the king the pawns tile is added to the checkmask.
 *  (ii)  Knight:
 *    If a knight checks the king the Knights tile is added to the checkmask
 *  (iii) Sliding Piece:
 *    If a sliding piece checks the king, the path from the enemy (inclusiv) to
 * the king (exclusiv) is added to the checkmask.
 *
 *  NOTE: There can only be two check per move.
 */
template <class BoardState state>
inline checkmask_t calculate_checkmask(const Board &board) {
  checkmask_t checkmask = 0;
  // pawn checkmask
  if constexpr (state.turn()) {
    const bitmap_t left_checking_pawns =
        (((board.enemy_pawns_of<state.turn()>() & (~rank8)) >> 7) &
         board.king_of<state.turn()>())
        << 7;
    checkmask |= left_checking_pawns;
    const bitmap_t right_checking_pawns =
        (((board.enemy_pawns_of<state.turn()>() & (~rank1)) >> 9) &
         board.king_of<state.turn()>())
        << 9;
    checkmask |= right_checking_pawns;
  } else {
    const bitmap_t left_checking_pawns =
        (((board.enemy_pawns_of<state.turn()>() & (~rank8)) << 9) &
         board.king_of<state.turn()>()) >>
        9;
    checkmask |= left_checking_pawns;
    const bitmap_t right_checking_pawns =
        (((board.enemy_pawns_of<state.turn()>() & (~rank1)) << 7) &
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

/**
 *  Calculates the pinmask.
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
 *  The pinmask has to be seperated into Horizontal & Vertical (HV) and
 *  Diagonal (D). If all pins would be stored in one pinmask a piece that in
 * pined horizontaly could move to the diagonal pinmask.
 *
 *  Behaviour for all pieces regarding the pinmask:
 *  (i)   Pawn:
 *    A hv-pined pawn can only move vertical, diagonal moves are not allowed.
 *    A d-pined pawn can only move diagonal in the direction of the pin.
 *  (ii)  Knight:
 *    A pined Knight can never move.
 *  (iii) Bishop & Rook:
 *    If a bishop is pined diagonaly it can only move on that diagonal.
 *    Rooks are analog with the hv-pinmask.
 *  (iv)  Queen:
 *    If a Queen is pined diagonaly it can only move diagonaly.
 *    If a Queen is pined hv it can only move horizontal or vertical.
 *
 *  NOTE: There can be upto 8 pins on one board.
 */
template <class BoardState state>
static inline pinmask_t calculate_pinmask(const Board &board) {
  pinmask_t pinmask;
  pinmask.hv = 0;
  pinmask.d = 0;

  // Calculate hv pinmask.
  const bitmap_t kingLookupSouth =
      SouthSlidingLookUpTable::get()[SQUARE_OF(board.king_of<state.turn()>())];
  const bitmap_t kingLookupWest =
      WestSlidingLookUpTable::get()[SQUARE_OF(board.king_of<state.turn()>())];
  const bitmap_t kingLookupNorth =
      NorthSlidingLookUpTable::get()[SQUARE_OF(board.king_of<state.turn()>())];
  const bitmap_t kingLookupEast =
      EastSlidingLookUpTable::get()[SQUARE_OF(board.king_of<state.turn()>())];
  bitmap_t enemyRooksAndQueens =
      board.enemy_rooks_and_queens_of<state.turn()>();
  iterate_bits(hvSliding, enemyRooksAndQueens) {
    const bitmap_t c0 =
        SouthSlidingLookUpTable::get()[SQUARE_OF(hvSliding)] & ~kingLookupSouth;
    pinmask.hv |= (((c0 & board.king_of<state.turn()>()) &&
                    c0 & board.occupied_by<state.turn()>()) &&
                   !(_blsr_u64(c0 & board.occupied_by<state.turn()>() & ~board.king_of<state.turn()>()))) *
                  ((c0 | hvSliding)& ~board.king_of<state.turn()>());

    const bitmap_t c1 =
        WestSlidingLookUpTable::get()[SQUARE_OF(hvSliding)] & ~kingLookupWest;
    pinmask.hv |= (((c1 & board.king_of<state.turn()>()) &&
                    (c1 & board.occupied_by<state.turn()>())) &&
                   !(_blsr_u64(c1 & board.occupied_by<state.turn()>() & ~board.king_of<state.turn()>()))) *
                  ((c1 | hvSliding) & ~board.king_of<state.turn()>());
    const bitmap_t c2 =
        NorthSlidingLookUpTable::get()[SQUARE_OF(hvSliding)] & ~kingLookupNorth;
    pinmask.hv |= (((c2 & board.king_of<state.turn()>()) &&
                    c2 & board.occupied_by<state.turn()>()) &&
                   !(_blsr_u64(c1 & board.occupied_by<state.turn()>() & ~board.king_of<state.turn()>()))) *
                  ((c2 | hvSliding) & ~board.king_of<state.turn()>());

    const bitmap_t c3 =
        EastSlidingLookUpTable::get()[SQUARE_OF(hvSliding)] & ~kingLookupEast;
    pinmask.hv |= (((c3 & board.king_of<state.turn()>()) &&
                    c3 & board.occupied_by<state.turn()>()) &&
                   !(_blsr_u64(c1 & board.occupied_by<state.turn()>() & ~board.king_of<state.turn()>()))) *
                  ((c3 | hvSliding) & ~board.king_of<state.turn()>());
  }
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
      board.enemy_bishop_and_queens_of<state.turn()>();
  iterate_bits(dSliding, enemyBishopsAndQueens) {
    const bitmap_t c0 =
        SouthEastSlidingLookUpTable::get()[SQUARE_OF(dSliding)] &
        ~kingLookupSouthEast;
    pinmask.d |= (((c0 & board.king_of<state.turn()>()) &&
                   c0 & board.occupied_by<state.turn()>()) &&
                  !(_blsr_u64(c0 & board.occupied_by<state.turn()>() & ~board.king_of<state.turn()>()))) *
                 ((c0 | dSliding) & ~board.king_of<state.turn()>());
    const bitmap_t c1 =
        SouthWestSlidingLookUpTable::get()[SQUARE_OF(dSliding)] &
        ~kingLookupSouthWest;
    pinmask.d |= (((c0 & board.king_of<state.turn()>()) &&
                   c1 & board.occupied_by<state.turn()>()) &&
                  !(_blsr_u64(c0 & board.occupied_by<state.turn()>() & ~board.king_of<state.turn()>()))) *
                 ((c1 | dSliding) & ~board.king_of<state.turn()>());
    const bitmap_t c2 =
        NorthWestSlidingLookUpTable::get()[SQUARE_OF(dSliding)] &
        ~kingLookupNorthWest;
    pinmask.d |= (((c2 & board.king_of<state.turn()>()) &&
                   c2 & board.occupied_by<state.turn()>()) &&
                  !(_blsr_u64(c0 & board.occupied_by<state.turn()>() & ~board.king_of<state.turn()>()))) *
                 ((c2 | dSliding) & ~board.king_of<state.turn()>());
    const bitmap_t c3 =
        NorthEastSlidingLookUpTable::get()[SQUARE_OF(dSliding)] &
        ~kingLookupNorthEast;
    pinmask.d |= (((c3 & board.king_of<state.turn()>()) &&
                   c3 & board.occupied_by<state.turn()>()) &&
                  !(_blsr_u64(c0 & board.occupied_by<state.turn()>() & ~board.king_of<state.turn()>()))) *
                 ((c3 | dSliding) & ~board.king_of<state.turn()>());
  }
  return pinmask;
}

/**
 * Calculates the banmask.
 *
 * The banmask includes all tiles that the enemy pieces attack.
 */
template <class BoardState state>
static inline banmask_t calculate_banmask(const Board &board) {
  // Calculate king banmask.
  // NOTE: will probably create a branch in asm. Could be removed if, with the
  // traidoff that only valid positions would be allowed.
  banmask_t banmask = 0;
  if (board.king_of<not state.turn()>()) {
    bitmap_t king_attacks =
        KingLookUpTable::get()[SQUARE_OF(board.king_of<not state.turn()>())] &
        ~board.occupied();
    banmask |= king_attacks;
  }

  // Calculate pawn attacks.
  bitmap_t pawns = board.pawns_of<not state.turn()>();
  if constexpr (state.turn()) {
    // pawn attack left.
    const bitmap_t left_attacking_pawns =
        (((pawns & ~(file1 | rank8)) << 7) &
         board.occupied_by<!state.turn()>()) >>
        7;
    banmask |= left_attacking_pawns << 7;
    // pawn attack right.
    const bitmap_t right_attacking_pawns =
        (((pawns & ~(file1 | rank8)) << 9) &
         board.occupied_by<!state.turn()>()) >>
        9;
    banmask |= right_attacking_pawns << 9;
  } else {
    // pawns attack right.
    const bitmap_t right_attacking_pawn =
        (((pawns & ~(file8 | rank1)) >> 7) & board.occupied_by<!state.turn()>())
        << 7;
    banmask |= right_attacking_pawn >> 7;
    // pawns attack left.
    const bitmap_t left_attacking_pawn =
        (((pawns & ~(file1 | rank1)) >> 9) & board.occupied_by<!state.turn()>())
        << 9;
    banmask |= left_attacking_pawn >> 9;
  }

  // Calculate knight attacks.

  bitmap_t knights = board.knights_of<state.turn()>();
  iterate_bits(knight, knights) {
    bitmap_t lookup = KnightLookUpTable::get()[SQUARE_OF(knight)] &
                      board.not_occupied();
    banmask |= lookup;
  }

  // Calculate hv sliding piece attacks.
  bitmap_t rooksAndQueens =
      board.rooks_and_queens_of<not state.turn()>();
  iterate_bits(hvSliding, rooksAndQueens) {
    // hvSliding to the left.
    const bitmap_t lookup1 = WestSlidingLookUpTable::get()[SQUARE_OF(hvSliding)];
    const bitmap_t hit1 =
        (lookup1 & board.occupied()) & (((bitmap_t)(-1)) << (-_lzcnt_u64(lookup1 & board.occupied()) - 1));
    banmask |= lookup1 & ~((!!hit1) * WestSlidingLookUpTable::get()[SQUARE_OF(hit1)]);

    const bitmap_t lookup2 = SouthSlidingLookUpTable::get()[SQUARE_OF(hvSliding)];
    const bitmap_t hit2 = (lookup2 & board.occupied()) & (((bitmap_t)-1)) << (-_lzcnt_u64(lookup2 & board.occupied()) -1 );
    banmask |= lookup2 & ~((!!hit2) * SouthSlidingLookUpTable::get()[SQUARE_OF(hit2)]);

    const bitmap_t lookup3 = NorthSlidingLookUpTable::get()[SQUARE_OF(hvSliding)];
    const bitmap_t hit3 = _blsi_u64(lookup3 & board.occupied());
    banmask |= lookup3 & ~((!!hit3) * NorthSlidingLookUpTable::get()[SQUARE_OF(hit3)]);

    const bitmap_t lookup4 = EastSlidingLookUpTable::get()[SQUARE_OF(hvSliding)];
    const bitmap_t hit4 = _blsi_u64(lookup4 & board.occupied());
    banmask |= lookup4 & ~((!!hit4) * EastSlidingLookUpTable::get()[SQUARE_OF(hit4)]);
  }

  // Calculate diagonal sliding piece attacks.
  bitmap_t bishopsAndQueens =
      board.bishop_and_queens_of<not state.turn()>();
  iterate_bits(dSliding, bishopsAndQueens){
    const bitmap_t lookup1 = SouthEastSlidingLookUpTable::get()[SQUARE_OF(dSliding)];
    const bitmap_t hit1 = (lookup1 & board.occupied()) & (((bitmap_t)-1) << (-_lzcnt_u64(lookup1 & board.occupied()) -1));
    banmask |= lookup1 & ~((!!hit1) * SouthEastSlidingLookUpTable::get()[SQUARE_OF(hit1)]);

    const bitmap_t lookup2 = SouthWestSlidingLookUpTable::get()[SQUARE_OF(dSliding)];
    const bitmap_t hit2 = (lookup2 & board.occupied()) & (((bitmap_t)-1) << (-_lzcnt_u64(lookup2 & board.occupied()) -1));
    banmask |= lookup2 & ~((!!hit2) * SouthEastSlidingLookUpTable::get()[SQUARE_OF(hit2)]);

    const bitmap_t lookup3 = NorthWestSlidingLookUpTable::get()[SQUARE_OF(dSliding)];
    const bitmap_t hit3 = _blsi_u64(lookup3 & board.occupied());
    banmask |= lookup3 & ~((!!hit3) * NorthWestSlidingLookUpTable::get()[SQUARE_OF(hit3)]);

    const bitmap_t lookup4 = NorthEastSlidingLookUpTable::get()[SQUARE_OF(dSliding)];
    const bitmap_t hit4 = _blsi_u64(lookup4 & board.occupied());
    banmask |= lookup4 & ~((!!hit4) * NorthEastSlidingLookUpTable::get()[SQUARE_OF(hit4)]);
  }

  return banmask;
}

/**
 *  Generate king moves.
 *
 *  @param <state> : the current state of the chess game.
 *  @param board : the chessboard including all the pieces.
 *  @param move_callback : a callback for moves.
 *  @param checkmask : the precalculated checkmask of the current turn.
 *
 *  iterates over all moves the king from the player who's turn it is and
 *  calls move_callback to indicate a possible move.
 */
template <class BoardState state>
static inline void generate_king_moves(const Board &board,
                                       void (*move_callback)(const move_t move),
                                       const bitmap_t checkmask, const bitmap_t banmask) {
  // NOT A CONSTEXPR
  // NOTE will probably create a branch in asm.
  if (!board.king_of<state.turn()>()) return;
  bitmap_t king_squares =
      KingLookUpTable::get()[SQUARE_OF(board.king_of<state.turn()>())] &
      board.empty_or_occupied_by_enemy_of<state.turn()>() & checkmask & ~banmask;
  iterate_bits(i, king_squares) {
    move_callback(move_t(board, board.king_of<state.turn()>(), i));
  }
}

/**
 *  Generate pawn moves.
 *
 *  @param <state> : the current state of the chess game.
 *  @param board : the chessboard including all the pieces.
 *  @param move_callback : a callback for moves.
 *  @param checkmask : the precalculated checkmask of the current turn.
 *  @param pinmask : the precalculated pinmask of the current turn.
 *
 *  iterates over all moves the pawn from the player who's turn it is and
 *  calls move_callback to indicate a possible move.
 */
template <class BoardState state>
static inline void generate_pawn_moves(const Board &board,
                                       void (*move_callback)(const move_t move),
                                       const bitmap_t checkmask,
                                       const pinmask_t pinmask) {
  const bitmap_t pawns = board.pawns_of<state.turn()>();
  if constexpr (state.turn()) {
    //bitmap_t unpined_pawns = (pawns & ~(pinmask.d | pinmask.hv));
    // pawns that can go forward.
    bitmap_t pushable_pawns =
        (((pawns & ~(rank7 | rank8)) << 8) & board.not_occupied()) >> 8;
    iterate_bits(pushable_pawn, pushable_pawns) {
      move_callback(move_t(board, pushable_pawn, pushable_pawn << 8));
    }
    // pawns that can double push.
    bitmap_t double_pushable_pawns =
        (((((pawns & rank2) << 8) & board.not_occupied()) << 8) &
         board.not_occupied()) >>
        16;
    iterate_bits(pushable_pawn, double_pushable_pawns) {
      move_callback(move_t(board, pushable_pawn, pushable_pawn << 16));
    }
    // pawn attack left.
    bitmap_t left_attacking_pawns = (((pawns & ~(file1 | rank8)) << 7) &
                                     board.occupied_by<!state.turn()>()) >>
                                    7;
    iterate_bits(attacking_pawn, left_attacking_pawns) {
      move_callback(move_t(board, attacking_pawn, attacking_pawn << 7));
    }
    // pawn attack right.
    bitmap_t right_attacking_pawns = (((pawns & ~(file1 | rank8)) << 9) &
                                      board.occupied_by<!state.turn()>()) >>
                                     9;
    iterate_bits(attacking_pawn, right_attacking_pawns) {
      move_callback(move_t(board, attacking_pawn, attacking_pawn << 9));
    }
  } else {
    // pawns that can go forward.
    bitmap_t pushable_pawns =
        (((pawns & ~(rank1 | rank2)) >> 8) & board.not_occupied()) << 8;
    iterate_bits(pushable_pawn, pushable_pawns) {
      move_callback(move_t(board, pushable_pawn, pushable_pawn >> 8));
    }
    // pawns that can double push.
    bitmap_t double_pushable_pawns =
        (((((pawns & rank2) >> 8) & board.not_occupied()) >> 8) &
         board.not_occupied())
        << 16;
    iterate_bits(pushable_pawn, double_pushable_pawns) {
      move_callback(move_t(board, pushable_pawn, pushable_pawn >> 16));
    }
    // pawns attack right.
    bitmap_t right_attacking_pawn =
        (((pawns & ~(file8 | rank1)) >> 7) & board.occupied_by<!state.turn()>())
        << 7;
    iterate_bits(attacking_pawn, right_attacking_pawn) {
      move_callback(move_t(board, attacking_pawn, attacking_pawn >> 7));
    }
    // pawns attack left.
    bitmap_t left_attacking_pawn =
        (((pawns & ~(file1 | rank1)) >> 9) & board.occupied_by<!state.turn()>())
        << 9;
    iterate_bits(attacking_pawn, left_attacking_pawn) {
      move_callback(move_t(board, attacking_pawn, attacking_pawn >> 9));
    }
  }
}

/**
 *  Generate knight moves.
 *
 *  @param <state> : the current state of the chess game.
 *  @param board : the chessboard including all the pieces.
 *  @param move_callback : a callback for moves.
 *  @param checkmask : the precalculated checkmask of the current turn.
 *  @param pinmask : the precalculated pinmask of the current turn.
 *
 *  iterates over all moves the knight from the player who's turn it is and
 *  calls move_callback to indicate a possible move.
 */
template <class BoardState state>
static inline void generate_knight_moves(
    const Board &board, void (*move_callback)(const move_t move),
    const bitmap_t checkmask, const pinmask_t pinmask) {
  bitmap_t knights = board.knights_of<state.turn()>() &
                     ~(pinmask.hv | pinmask.d);  // pinned knight can't move
  iterate_bits(knight, knights) {
    bitmap_t lookup = KnightLookUpTable::get()[SQUARE_OF(knight)] &
                      board.empty_or_occupied_by_enemy_of<state.turn()>() &
                      checkmask;
    iterate_bits(att, lookup) { move_callback(move_t(board, knight, att)); }
  }
}

/**
 *  Generate sliding moves.
 *
 *  @param <state> : the current state of the chess game.
 *  @param board : the chessboard including all the pieces.
 *  @param move_callback : a callback for moves.
 *  @param checkmask : the precalculated checkmask of the current turn.
 *  @param pinmask : the precalculated pinmask of the current turn.
 *
 *  iterates over all moves the sliding from the player who's turn it is and
 *  calls move_callback to indicate a possible move.
 */
template <class BoardState state>
static inline void generate_sliding_moves(
    const Board &board, void (*move_callback)(const move_t move),
    const bitmap_t checkmask, const pinmask_t pinmask) {
  bitmap_t itBits;

  bitmap_t rooksAndQueues =
      board.rooks_and_queens_of<state.turn()>() & ~(pinmask.hv | pinmask.d);
  bitmap_t bishopsAndQueens =
      board.bishop_and_queens_of<state.turn()>() & ~(pinmask.hv | pinmask.d);

  // Calculate sliding moves to the left.
  itBits = rooksAndQueues;
  iterate_bits(dsliding, itBits) {
    const bitmap_t lookup = WestSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
    const bitmap_t enemiesInPath =
        lookup & board.occupied_by_enemy_of<state.turn()>();
    const bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
    const bitmap_t inPath = (aliesInPath << ((bitmap_t)1)) | enemiesInPath;
    const bitmap_t hit =
        inPath & (((bitmap_t)(-1)) << (-_lzcnt_u64(inPath) - 1));
    bitmap_t possibleMoves =
        ~WestSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
    iterate_bits(target, possibleMoves) {
      move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves to the right.
  itBits = rooksAndQueues;
  iterate_bits(dsliding, itBits) {
    const bitmap_t lookup = EastSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
    const bitmap_t enemiesInPath =
        lookup & board.occupied_by_enemy_of<state.turn()>();
    const bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
    const bitmap_t inPath = (aliesInPath >> ((bitmap_t)1)) | enemiesInPath;
    const bitmap_t hit = _blsi_u64(inPath);  // extract lowest bit
    bitmap_t possibleMoves =
        ~EastSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
    iterate_bits(target, possibleMoves) {
      move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves upwards.
  itBits = rooksAndQueues;
  iterate_bits(dsliding, itBits) {
    const bitmap_t lookup = NorthSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
    const bitmap_t enemiesInPath =
        lookup & board.occupied_by_enemy_of<state.turn()>();
    const bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
    const bitmap_t inPath = (aliesInPath >> ((bitmap_t)8)) | enemiesInPath;
    const bitmap_t hit = _blsi_u64(inPath);  // extract lowest bit
    bitmap_t possibleMoves =
        ~NorthSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
    iterate_bits(target, possibleMoves) {
      move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves downwards.
  itBits = rooksAndQueues;
  iterate_bits(dsliding, itBits) {
    const bitmap_t lookup = SouthSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
    const bitmap_t enemiesInPath =
        lookup & board.occupied_by_enemy_of<state.turn()>();
    const bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
    const bitmap_t inPath = (aliesInPath << ((bitmap_t)8)) | enemiesInPath;
    const bitmap_t hit =
        inPath & (((bitmap_t)(-1)) << (-_lzcnt_u64(inPath) - 1));
    bitmap_t possibleMoves =
        ~SouthSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
    iterate_bits(target, possibleMoves) {
      move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves down right diagonal
  itBits = bishopsAndQueens;
  iterate_bits(dsliding, itBits) {
    const bitmap_t lookup =
        SouthEastSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
    const bitmap_t enemiesInPath =
        lookup & board.occupied_by_enemy_of<state.turn()>();
    const bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
    const bitmap_t inPath = (aliesInPath << ((bitmap_t)7)) | enemiesInPath;
    const bitmap_t hit =
        inPath & (((bitmap_t)(-1)) << (-_lzcnt_u64(inPath) - 1));
    bitmap_t possibleMoves =
        ~SouthEastSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
    iterate_bits(target, possibleMoves) {
      move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves down left diagonal
  itBits = bishopsAndQueens;
  iterate_bits(dsliding, itBits) {
    const bitmap_t lookup =
        SouthWestSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
    const bitmap_t enemiesInPath =
        lookup & board.occupied_by_enemy_of<state.turn()>();
    const bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
    const bitmap_t inPath = (aliesInPath << ((bitmap_t)9)) | enemiesInPath;
    const bitmap_t hit =
        inPath & (((bitmap_t)(-1)) << (-_lzcnt_u64(inPath) - 1));
    bitmap_t possibleMoves =
        ~SouthWestSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
    iterate_bits(target, possibleMoves) {
      move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves up left diagonal
  itBits = bishopsAndQueens;
  iterate_bits(dsliding, itBits) {
    const bitmap_t lookup =
        NorthWestSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
    const bitmap_t enemiesInPath =
        lookup & board.occupied_by_enemy_of<state.turn()>();
    const bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
    const bitmap_t inPath = (aliesInPath >> ((bitmap_t)7)) | enemiesInPath;
    const bitmap_t hit = _blsi_u64(inPath);  // extract lowest bit
    bitmap_t possibleMoves =
        ~NorthWestSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
    iterate_bits(target, possibleMoves) {
      move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves up right diagonal
  itBits = bishopsAndQueens;
  iterate_bits(dsliding, itBits) {
    const bitmap_t lookup =
        NorthEastSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
    const bitmap_t enemiesInPath =
        lookup & board.occupied_by_enemy_of<state.turn()>();
    const bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
    const bitmap_t inPath = (aliesInPath >> ((bitmap_t)9)) | enemiesInPath;
    const bitmap_t hit = _blsi_u64(inPath);  // extract lowest bit
    bitmap_t possibleMoves =
        ~NorthEastSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
    iterate_bits(target, possibleMoves) {
      move_callback(move_t(board, dsliding, target));
    }
  }
}
