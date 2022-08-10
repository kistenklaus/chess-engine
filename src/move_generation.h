#pragma once

#include "Board.h"
#include "BoardState.h"
#include "bitmap.h"
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

template <class BoardState state>
inline void generate_moves(const Board &board,
                           void (*move_callback)(const move_t move)) {
  bitmap_t itBits;

  const bitmap_t king = board.king_of<state.turn()>();

  bitmap_t checkmask = 0;
  // CALCULATE CHECKMASK.

  // calculate pawnmoves.
  {  // local scope
    const bitmap_t enemyPawns = board.enemy_pawns_of<state.turn()>();
    if constexpr (state.turn()) {
      std::cout << bitmap_to_bitboard_string((enemyPawns & (~rank8)) << 7)
                << std::endl;
      bitmap_t left_checking_pawns = (((enemyPawns & (~rank8)) >> 7) & king)
                                     << 7;
      bitmap_t right_checking_pawns = (((enemyPawns & (~rank1)) >> 9) & king)
                                      << 9;
      checkmask |= left_checking_pawns;
      checkmask |= right_checking_pawns;
    } else {
      bitmap_t left_checking_pawns =
          (((enemyPawns & (~rank8)) << 9) & king) >> 9;
      bitmap_t right_checking_pawns =
          (((enemyPawns & (~rank1)) << 7) & king) >> 7;
      checkmask |= left_checking_pawns;
      checkmask |= right_checking_pawns;
    }

    // calculate knightmoves.
    bitmap_t enemyKnights = board.enemy_knights_of<state.turn()>();
    iterate_bits(knight, enemyKnights) {
      if (king &
          KnightLookUpTable::get()[SQUARE_OF(knight)]) {  // ahhh if statements
        checkmask |= knight;
      }
    }

    const bitmap_t enemyRooksAndQueens =
        board.rooks_and_queens_of<state.turn()>();

    const bitmap_t enemyBishopsAndQueens =
        board.bishop_and_queens_of<state.turn()>();
  }

  if (!checkmask) checkmask = -1;

  std::cout << "checkmask" << std::endl;
  std::cout << bitmap_to_bitboard_string(checkmask) << std::endl;

  // calculate pinmask.
  bitmap_t h_pinmask = 0;
  bitmap_t d_pinmask = 0;

  // CALCULATE MOVES.

  // calculate kingmoves.
  // fuuuuck a if statement i hate those.
  {
    if (king) {
      bitmap_t king_squares =
          KingLookUpTable::get()[SQUARE_OF(king)] &
          board.empty_or_occupied_by_enemy_of<state.turn()>() & checkmask;
      iterate_bits(i, king_squares) { move_callback(move_t(board, king, i)); }
    }

    // calculate pawnmoves.
    bitmap_t pawns = board.pawns_of<state.turn()>();
    if constexpr (state.turn()) {
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
      bitmap_t right_attacking_pawn = (((pawns & ~(file8 | rank1)) >> 7) &
                                       board.occupied_by<!state.turn()>())
                                      << 7;
      iterate_bits(attacking_pawn, right_attacking_pawn) {
        move_callback(move_t(board, attacking_pawn, attacking_pawn >> 7));
      }

      // pawns attack left.
      bitmap_t left_attacking_pawn = (((pawns & ~(file1 | rank1)) >> 9) &
                                      board.occupied_by<!state.turn()>())
                                     << 9;
      iterate_bits(attacking_pawn, left_attacking_pawn) {
        move_callback(move_t(board, attacking_pawn, attacking_pawn >> 9));
      }
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

    // calculate horizontal / vertical sliding moves.
    bitmap_t rooksAndQueues =
        board.rooks_and_queens_of<state.turn()>() & ~(h_pinmask | d_pinmask);

    bitmap_t bishopsAndQueens =
        board.bishop_and_queens_of<state.turn()>() & ~(h_pinmask | d_pinmask);

    itBits = rooksAndQueues;
    iterate_bits(dsliding, itBits) {
      bitmap_t lookup = WestSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
      bitmap_t enemiesInPath =
          lookup & board.occupied_by_enemy_of<state.turn()>();
      bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
      bitmap_t inPath = (aliesInPath << ((bitmap_t)1)) | enemiesInPath;
      bitmap_t hit = inPath & (((bitmap_t)(-1)) << (-_lzcnt_u64(inPath) - 1));
      bitmap_t possibleMoves =
          ~WestSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
      iterate_bits(target, possibleMoves) {
        move_callback(move_t(board, dsliding, target));
      }
    }

    itBits = rooksAndQueues;
    iterate_bits(dsliding, itBits) {
      bitmap_t lookup = EastSlidingLookUpTable::get()[SQUARE_OF(dsliding)];

      // hit enemy
      bitmap_t enemiesInPath =
          lookup & board.occupied_by_enemy_of<state.turn()>();
      bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
      bitmap_t inPath = (aliesInPath >> ((bitmap_t)1)) | enemiesInPath;
      bitmap_t hit = _blsi_u64(inPath);
      bitmap_t possibleMoves =
          ~EastSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
      iterate_bits(target, possibleMoves) {
        move_callback(move_t(board, dsliding, target));
      }
    }

    itBits = rooksAndQueues;
    iterate_bits(dsliding, itBits) {
      bitmap_t lookup = NorthSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
      bitmap_t enemiesInPath =
          lookup & board.occupied_by_enemy_of<state.turn()>();
      bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
      bitmap_t inPath = (aliesInPath >> ((bitmap_t)8)) | enemiesInPath;
      bitmap_t hit = _blsi_u64(inPath);
      bitmap_t possibleMoves =
          ~NorthSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
      iterate_bits(target, possibleMoves) {
        move_callback(move_t(board, dsliding, target));
      }
    }

    itBits = rooksAndQueues;
    iterate_bits(dsliding, itBits) {
      bitmap_t lookup = SouthSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
      bitmap_t enemiesInPath =
          lookup & board.occupied_by_enemy_of<state.turn()>();
      bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
      bitmap_t inPath = (aliesInPath << ((bitmap_t)8)) | enemiesInPath;
      bitmap_t hit = inPath & (((bitmap_t)(-1)) << (-_lzcnt_u64(inPath) - 1));
      bitmap_t possibleMoves =
          ~SouthSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
      iterate_bits(target, possibleMoves) {
        move_callback(move_t(board, dsliding, target));
      }
    }

    itBits = bishopsAndQueens;
    iterate_bits(dsliding, itBits) {
      bitmap_t lookup = SouthEastSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
      bitmap_t enemiesInPath =
          lookup & board.occupied_by_enemy_of<state.turn()>();
      bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
      bitmap_t inPath = (aliesInPath << ((bitmap_t)7)) | enemiesInPath;
      bitmap_t hit = inPath & (((bitmap_t)(-1)) << (-_lzcnt_u64(inPath) - 1));
      bitmap_t possibleMoves =
          ~SouthEastSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
      iterate_bits(target, possibleMoves) {
        move_callback(move_t(board, dsliding, target));
      }
    }

    itBits = bishopsAndQueens;
    iterate_bits(dsliding, itBits) {
      bitmap_t lookup = SouthWestSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
      bitmap_t enemiesInPath =
          lookup & board.occupied_by_enemy_of<state.turn()>();
      bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
      bitmap_t inPath = (aliesInPath << ((bitmap_t)9)) | enemiesInPath;
      bitmap_t hit = inPath & (((bitmap_t)(-1)) << (-_lzcnt_u64(inPath) - 1));
      bitmap_t possibleMoves =
          ~SouthWestSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
      iterate_bits(target, possibleMoves) {
        move_callback(move_t(board, dsliding, target));
      }
    }

    itBits = bishopsAndQueens;
    iterate_bits(dsliding, itBits) {
      bitmap_t lookup = NorthWestSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
      bitmap_t enemiesInPath =
          lookup & board.occupied_by_enemy_of<state.turn()>();
      bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
      bitmap_t inPath = (aliesInPath >> ((bitmap_t)7)) | enemiesInPath;
      bitmap_t hit = _blsi_u64(inPath);
      bitmap_t possibleMoves =
          ~NorthWestSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
      iterate_bits(target, possibleMoves) {
        move_callback(move_t(board, dsliding, target));
      }
    }

    itBits = bishopsAndQueens;
    iterate_bits(dsliding, itBits) {
      bitmap_t lookup = NorthEastSlidingLookUpTable::get()[SQUARE_OF(dsliding)];
      bitmap_t enemiesInPath =
          lookup & board.occupied_by_enemy_of<state.turn()>();
      bitmap_t aliesInPath = lookup & (board.occupied_by<state.turn()>());
      bitmap_t inPath = (aliesInPath >> ((bitmap_t)9)) | enemiesInPath;
      bitmap_t hit = _blsi_u64(inPath);
      bitmap_t possibleMoves =
          ~NorthEastSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
      iterate_bits(target, possibleMoves) {
        move_callback(move_t(board, dsliding, target));
      }
    }
  }
}
