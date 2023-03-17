#pragma once

#include <x86intrin.h>

#include <functional>

#include "Board.h"
#include "BoardState.h"
#include "banmask.h"
#include "banmask_generation.h"
#include "bitmap.h"
#include "build_config.h"
#include "checkmask.h"
#include "checkmask_generation.h"
#include "lookup.h"
#include "move.h"
#include "pinmask.h"
#include "pinmask_generation.h"
#include "x86utils.h"

template <class BoardState state, typename MoveReceiver>
inline void generate_moves(const Board &board, MoveReceiver &receiver) {
  const checkmask_t checkmask = calculate_checkmask<state>(board);
  const pinmask_t pinmask = generate_pinmask<state>(board);
  const banmask_t banmask = calculate_banmask<state>(board);
  generate_king_moves<state, MoveReceiver>(board, receiver, checkmask, banmask);
  generate_pawn_moves<state, MoveReceiver>(board, receiver, checkmask, pinmask);
  generate_knight_moves<state, MoveReceiver>(board, receiver, checkmask,
                                             pinmask);
  generate_dSliding_moves<state, MoveReceiver, BISHOP>(
      board, receiver, checkmask, pinmask, board.bishops_of<state.turn()>());
  generate_dSliding_moves<state, MoveReceiver, QUEEN>(
      board, receiver, checkmask, pinmask, board.queens_of<state.turn()>());
  generate_hvSliding_moves<state, MoveReceiver, ROOK>(
      board, receiver, checkmask, pinmask, board.rooks_of<state.turn()>());
  generate_hvSliding_moves<state, MoveReceiver, QUEEN>(
      board, receiver, checkmask, pinmask, board.queens_of<state.turn()>());
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
template <class BoardState state, typename MoveReceiver>
static inline void generate_king_moves(const Board &board,
                                       MoveReceiver &receiver,
                                       const bitmap_t checkmask,
                                       const bitmap_t banmask) {
  // NOT A CONSTEXPR
  // NOTE will probably create a branch in asm.
  if (!board.king_of<state.turn()>()) return;
  bitmap_t king_squares =
      KingLookUpTable::get()[SQUARE_OF(board.king_of<state.turn()>())] &
      board.empty_or_occupied_by_enemy_of<state.turn()>() & checkmask &
      ~banmask;
  iterate_bits(i, king_squares) {
    receiver.template move<KING>(board.king_of<state.turn()>(), i,
                                 MOVE_FLAG_NONE);
    // move_callback(move_t(board, board.king_of<state.turn()>(), i));
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
template <class BoardState state, typename MoveReceiver>
static inline void generate_pawn_moves(const Board &board,
                                       MoveReceiver &receiver,
                                       const bitmap_t checkmask,
                                       const pinmask_t pinmask) {
  const bitmap_t pawns = board.pawns_of<state.turn()>();
  if constexpr (state.turn()) {
    // bitmap_t unpinned_pawns = (pawns & ~(pinmask.d | pinmask.hv));
    //  pawns that can go forward.
    bitmap_t pushable_pawns = (((pawns & ~(Board::Rank7 | Board::Rank8)) << 8) &
                               board.not_occupied()) >>
                              8;
    iterate_bits(pushable_pawn, pushable_pawns) {
      receiver.template move<PAWN>(pushable_pawn, (pushable_pawn << 8),
                                   MOVE_FLAG_NONE);
      // move_callback(move_t(board, pushable_pawn, pushable_pawn << 8));
    }
    // pawns that can double push.
    bitmap_t double_pushable_pawns =
        (((((pawns & Board::Rank2) << 8) & board.not_occupied()) << 8) &
         board.not_occupied()) >>
        16;
    iterate_bits(pushable_pawn, double_pushable_pawns) {
      receiver.template move<PAWN>(pushable_pawn, pushable_pawn << 16,
                                   MOVE_FLAG_DOUBLE_PAWN_PUSH);
    }
    // pawn attack left.
    bitmap_t left_attacking_pawns =
        (((pawns & ~(Board::File1 | Board::Rank8)) << 7) &
         board.occupied_by<!state.turn()>()) >>
        7;
    iterate_bits(attacking_pawn, left_attacking_pawns) {
      receiver.template move<PAWN>(attacking_pawn, attacking_pawn << 7,
                                   MOVE_FLAG_CAPTURE);
    }
    // pawn attack right.
    bitmap_t right_attacking_pawns =
        (((pawns & ~(Board::File1 | Board::Rank8)) << 9) &
         board.occupied_by<!state.turn()>()) >>
        9;
    iterate_bits(attacking_pawn, right_attacking_pawns) {
      receiver.template move<PAWN>(attacking_pawn, attacking_pawn << 9,
                                   MOVE_FLAG_CAPTURE);
    }
  } else {
    // pawns that can go forward.
    bitmap_t pushable_pawns =
        (((pawns & ~(Board::Rank1 | Board::Rank2)) >> 8) & board.not_occupied())
        << 8;
    iterate_bits(pushable_pawn, pushable_pawns) {
      receiver.template move<PAWN>(pushable_pawn, pushable_pawn >> 8,
                                   MOVE_FLAG_NONE);
    }
    // pawns that can double push.
    bitmap_t double_pushable_pawns =
        (((((pawns & Board::Rank7) >> 8) & board.not_occupied()) >> 8) &
         board.not_occupied())
        << 16;
    iterate_bits(pushable_pawn, double_pushable_pawns) {
      receiver.template move<PAWN>(pushable_pawn, pushable_pawn >> 16,
                                   MOVE_FLAG_DOUBLE_PAWN_PUSH);
    }
    // pawns attack right.
    bitmap_t right_attacking_pawn =
        (((pawns & ~(Board::File8 | Board::Rank1)) >> 7) &
         board.occupied_by<!state.turn()>())
        << 7;
    iterate_bits(attacking_pawn, right_attacking_pawn) {
      receiver.template move<PAWN>(attacking_pawn, attacking_pawn >> 7,
                                   MOVE_FLAG_CAPTURE);
    }
    // pawns attack left.
    bitmap_t left_attacking_pawn =
        (((pawns & ~(Board::File1 | Board::Rank1)) >> 9) &
         board.occupied_by<!state.turn()>())
        << 9;
    iterate_bits(attacking_pawn, left_attacking_pawn) {
      receiver.template move<PAWN>(attacking_pawn, attacking_pawn >> 9,
                                   MOVE_FLAG_CAPTURE);
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
template <class BoardState state, typename MoveReceiver>
static inline void generate_knight_moves(const Board &board,
                                         MoveReceiver &receiver,
                                         const bitmap_t checkmask,
                                         const pinmask_t pinmask) {
  bitmap_t knights = board.knights_of<state.turn()>() &
                     ~(pinmask.hv | pinmask.d);  // pinned knight can't move
  iterate_bits(knight, knights) {
    bitmap_t lookup = KnightLookUpTable::get()[SQUARE_OF(knight)] &
                      board.empty_or_occupied_by_enemy_of<state.turn()>() &
                      checkmask;
    iterate_bits(att, lookup) {
      // TODO determine MOVE_HINT_CAPTURE
      // TODO add receiver back in
      receiver.template move<KNIGHT>(knight, att, MOVE_FLAG_NONE);
      // move_callback(move_t(board, knight, att));
    }
  }
}

template <class BoardState state, typename MoveReceiver, figure_type figure>
static inline void generate_hvSliding_moves(const Board &board,
                                            MoveReceiver &receiver,
                                            const bitmap_t checkmask,
                                            const pinmask_t pinmask,
                                            const bitmap_t slidingPieces) {
  // Calculate sliding moves to the left.
  bitmap_t itBits = slidingPieces;
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
      // TODO add capture hint.
      // TODO add check hint.
      receiver.template move<figure>(dsliding, target, MOVE_FLAG_NONE);
      // move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves to the right.
  itBits = slidingPieces;
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
      // TODO add capture hint
      receiver.template move<figure>(dsliding, target, MOVE_FLAG_NONE);
      // move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves upwards.
  itBits = slidingPieces;
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
      // TODO add capture hint
      receiver.template move<figure>(dsliding, target, MOVE_FLAG_NONE);
      // move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves downwards.
  itBits = slidingPieces;
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
      // TODO add capture hint.
      receiver.template move<figure>(dsliding, target, MOVE_FLAG_NONE);
      // move_callback(move_t(board, dsliding, target));
    }
  }
}

template <class BoardState state, typename MoveReceiver, figure_type figure>
static inline void generate_dSliding_moves(const Board &board,
                                           MoveReceiver &receiver,
                                           const bitmap_t checkmask,
                                           const pinmask_t pinmask,
                                           const bitmap_t slidingPieces) {
  // Calculate sliding moves down right diagonal
  bitmap_t itBits = slidingPieces;
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
      // TODO add capture hint.
      receiver.template move<figure>(dsliding, target, MOVE_FLAG_NONE);
      // move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves down left diagonal
  itBits = slidingPieces;
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
      // TODO add receiver callback.
      receiver.template move<figure>(dsliding, target, MOVE_FLAG_NONE);
      // move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves up left diagonal
  itBits = slidingPieces;
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
      // TODO add capture hint.
      receiver.template move<figure>(dsliding, target, MOVE_FLAG_NONE);
      // move_callback(move_t(board, dsliding, target));
    }
  }

  // Calculate sliding moves up right diagonal
  itBits = slidingPieces;
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
      // TODO add capture hint.
      receiver.template move<figure>(dsliding, target, MOVE_FLAG_NONE);
      // move_callback(move_t(board, dsliding, target));
    }
  }
}

/*
template <class BoardState state, typename MoveReceiver>
static inline void generate_sliding_moves(const Board &board,
                                          MoveReceiver receiver,
                                          const bitmap_t checkmask,
                                          const pinmask_t pinmask) {
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
      // TODO add receiver callback
      // move_callback(move_t(board, dsliding, target));
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
      // TODO add receiver callback.
      // move_callback(move_t(board, dsliding, target));
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
      // TODO add receiver callback
      // move_callback(move_t(board, dsliding, target));
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
      // TODO add receiver callback
      // move_callback(move_t(board, dsliding, target));
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
      // TODO add receiver callback.
      // move_callback(move_t(board, dsliding, target));
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
      // TODO add receiver callback.
      // move_callback(move_t(board, dsliding, target));
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
      // TODO add receiver callback.
      // move_callback(move_t(board, dsliding, target));
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
      // TODO add receiver callback.
      // move_callback(move_t(board, dsliding, target));
    }
  }
}

 */