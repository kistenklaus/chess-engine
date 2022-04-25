
#pragma once

#include "Board.h"
#include "BoardState.h"
#include "bitmap.h"
#include "lookup.h"
#include "move.h"
#include "x86utils.h"

template <class BoardState state>
inline void generate_moves(const Board& board,
                           void (*move_callback)(const move_t& move)) {
  bitmap_t checkmask = ~0;
  // calculate checkmask.
  bitmap_t h_pinmask = 0;
  bitmap_t d_pinmask = 0;
  // calculate pinmask.

  // calculate kingmoves.
  const bitmap_t king = board.king_of<state.turn()>();
  // fuuuuck a if statement i hate those.
  if (king) {
    bitmap_t king_squares =
        KingLookUpTable::get()[SQUARE_OF(king)] &
        board.empty_or_occupied_by_enemy_of<state.turn()>() & checkmask;
    iterate_bits(i, king_squares) { move_callback(move_t(board, king, i)); }
  }

  // calculate pawnmoves.
  bitmap_t pawns = board.pawns_of<state.turn()>();
  if constexpr (state.turn()) {
  } else {
  }

  // calculate knightmoves.
  bitmap_t knights = board.knights_of<state.turn()>() &
                     ~(h_pinmask | d_pinmask);  // pinned knight can't move
  iterate_bits(knight, knights) {
    bitmap_t lookup = KnightLookUpTable::get()[SQUARE_OF(knight)] &
                      board.empty_or_occupied_by_enemy_of<state.turn()>() &
                      checkmask;
    iterate_bits(att, lookup) { move_callback(move_t(board, knight, att)); }
  }

  bitmap_t bishops = board.bishops_of<state.turn()>();
  iterate_bits(bishop, bishops) {
    bitmap_t lookup = DSlidingLookUpTable::get()[SQUARE_OF(bishop)] &
                      board.empty_or_occupied_by_enemy_of<state.turn()>() &
                      checkmask;
    // iterate_bits(att, lookup) {}
  }

  bitmap_t rooks = board.rooks_of<state.turn()>();
  iterate_bits(rook, rooks) {
    bitmap_t lookup = HVSlidingLookUpTable::get()[SQUARE_OF(rook)];
    // iterate_bits(att, lookup) {}
  }
}
