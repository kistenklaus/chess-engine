#pragma once

#include <x86intrin.h>

#include <functional>

#include "Board.h"
#include "BoardState.h"
#include "banmask.h"
#include "bitmap.h"
#include "build_config.h"
#include "checkmask.h"
#include "checkmask_generation.h"
#include "banmask_generation.h"
#include "pinmask_generation.h"
#include "lookup.h"
#include "move.h"
#include "pinmask.h"
#include "x86utils.h"

template <class BoardState state>
inline void generate_moves(
    const Board &board,
    const std::function<void(const move_t)> &move_callback) {
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
  generate_king_moves<state>(board, move_callback, checkmask, banmask);
  generate_pawn_moves<state>(board, move_callback, checkmask, pinmask);
  generate_knight_moves<state>(board, move_callback, checkmask, pinmask);
  generate_sliding_moves<state>(board, move_callback, checkmask, pinmask);
}

/**
 *  Generate King moves.
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
static inline void generate_king_moves(
    const Board &board, const std::function<void(const move_t)> &move_callback,
    const bitmap_t checkmask, const bitmap_t banmask) {
  // NOT A CONSTEXPR
  // NOTE will probably create a branch in asm.
  if (!board.king_of<state.turn()>()) return;
  bitmap_t king_squares =
      KingLookUpTable::get()[SQUARE_OF(board.king_of<state.turn()>())] &
      board.empty_or_occupied_by_enemy_of<state.turn()>() & checkmask &
      ~banmask;
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
static inline void generate_pawn_moves(
    const Board &board, const std::function<void(const move_t)> &move_callback,
    const bitmap_t checkmask, const pinmask_t pinmask) {
  const bitmap_t pawns = board.pawns_of<state.turn()>();
  if constexpr (state.turn()) {
    // bitmap_t unpinned_pawns = (pawns & ~(pinmask.d | pinmask.hv));
    //  pawns that can go forward.
    bitmap_t pushable_pawns = (((pawns & ~(Board::Rank7 | Board::Rank8)) << 8) &
                               board.not_occupied()) >>
                              8;
    iterate_bits(pushable_pawn, pushable_pawns) {
      move_callback(move_t(board, pushable_pawn, pushable_pawn << 8));
    }
    // pawns that can double push.
    bitmap_t double_pushable_pawns =
        (((((pawns & Board::Rank2) << 8) & board.not_occupied()) << 8) &
         board.not_occupied()) >>
        16;
    iterate_bits(pushable_pawn, double_pushable_pawns) {
      move_callback(move_t(board, pushable_pawn, pushable_pawn << 16));
    }
    // pawn attack left.
    bitmap_t left_attacking_pawns =
        (((pawns & ~(Board::File1 | Board::Rank8)) << 7) &
         board.occupied_by<!state.turn()>()) >>
        7;
    iterate_bits(attacking_pawn, left_attacking_pawns) {
      move_callback(move_t(board, attacking_pawn, attacking_pawn << 7));
    }
    // pawn attack right.
    bitmap_t right_attacking_pawns =
        (((pawns & ~(Board::File1 | Board::Rank8)) << 9) &
         board.occupied_by<!state.turn()>()) >>
        9;
    iterate_bits(attacking_pawn, right_attacking_pawns) {
      move_callback(move_t(board, attacking_pawn, attacking_pawn << 9));
    }
  } else {
    // pawns that can go forward.
    bitmap_t pushable_pawns =
        (((pawns & ~(Board::Rank1 | Board::Rank2)) >> 8) & board.not_occupied())
        << 8;
    iterate_bits(pushable_pawn, pushable_pawns) {
      move_callback(move_t(board, pushable_pawn, pushable_pawn >> 8));
    }
    // pawns that can double push.
    bitmap_t double_pushable_pawns =
        (((((pawns & Board::Rank2) >> 8) & board.not_occupied()) >> 8) &
         board.not_occupied())
        << 16;
    iterate_bits(pushable_pawn, double_pushable_pawns) {
      move_callback(move_t(board, pushable_pawn, pushable_pawn >> 16));
    }
    // pawns attack right.
    bitmap_t right_attacking_pawn =
        (((pawns & ~(Board::File8 | Board::Rank1)) >> 7) &
         board.occupied_by<!state.turn()>())
        << 7;
    iterate_bits(attacking_pawn, right_attacking_pawn) {
      move_callback(move_t(board, attacking_pawn, attacking_pawn >> 7));
    }
    // pawns attack left.
    bitmap_t left_attacking_pawn =
        (((pawns & ~(Board::File1 | Board::Rank1)) >> 9) &
         board.occupied_by<!state.turn()>())
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
    const Board &board, const std::function<void(const move_t)> &move_callback,
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
    const Board &board, const std::function<void(const move_t)> &move_callback,
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
    const bitmap_t hit = _blsi_u64(inPath);  // extract the lowest bit
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
    const bitmap_t hit = _blsi_u64(inPath);  // extract the lowest bit
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
    const bitmap_t hit = _blsi_u64(inPath);  // extract the lowest bit
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
    const bitmap_t hit = _blsi_u64(inPath);  // extract the lowest bit
    bitmap_t possibleMoves =
        ~NorthEastSlidingLookUpTable::get()[SQUARE_OF(hit)] & lookup;
    iterate_bits(target, possibleMoves) {
      move_callback(move_t(board, dsliding, target));
    }
  }
}
