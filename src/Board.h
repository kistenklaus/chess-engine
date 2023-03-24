#pragma once
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "GameState.h"
#include "bitmap.h"
#include "cinttypes"
#include "figure.h"
#include "move.h"

class Board {
 public:
  const static bitmap_t Rank7 = 0xFF000000000000;
  const static bitmap_t Rank8 = 0xFF00000000000000;
  const static bitmap_t Rank1 = 0xFF;
  const static bitmap_t Rank2 = 0xFF00;
  const static bitmap_t File8 = 0x8080808080808080;
  const static bitmap_t File7 = File8 >> 1;
  const static bitmap_t File2 = File7 >> 5;
  const static bitmap_t File1 = File2 >> 1;

 private:
  bitmap_t b_occ;
  bitmap_t w_occ;
  bitmap_t occ;
  bitmap_t b_pawns;
  bitmap_t w_pawns;
  bitmap_t b_bishops;
  bitmap_t w_bishops;
  bitmap_t b_knights;
  bitmap_t w_knights;
  bitmap_t b_rooks;
  bitmap_t w_rooks;
  bitmap_t w_king;
  bitmap_t b_king;
  bitmap_t w_queen;
  bitmap_t b_queen;

 public:
  explicit Board(bitmap_t b_pawns, bitmap_t w_pawns, bitmap_t b_bishops,
                 bitmap_t w_bishops, bitmap_t b_knights, bitmap_t w_knights,
                 bitmap_t b_rooks, bitmap_t w_rooks, bitmap_t b_queen,
                 bitmap_t w_queen, bitmap_t b_king, bitmap_t w_king)
      : b_occ(b_pawns | b_bishops | b_knights | b_rooks | b_queen | b_king),
        w_occ(w_pawns | w_bishops | w_knights | w_rooks | w_queen | w_king),
        occ(w_occ | b_occ),
        b_pawns(b_pawns),
        w_pawns(w_pawns),
        b_bishops(b_bishops),
        w_bishops(w_bishops),
        b_knights(b_knights),
        w_knights(w_knights),
        b_rooks(b_rooks),
        w_rooks(w_rooks),
        w_king(w_king),
        b_king(b_king),
        w_queen(w_queen),
        b_queen(b_queen)  // probably doesn't work
  {}

  [[nodiscard]] constexpr inline bitmap_t Occupied() const { return occ; }

  [[nodiscard]] constexpr inline bitmap_t NotOccupied() const { return ~occ; }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t EnemyOrEmpty()
      const {
    if constexpr (current_player)
      return ~w_occ;
    else
      return ~b_occ;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t rooks_and_queens_of() const {
    if constexpr (current_player)
      return w_rooks | w_queen;
    else
      return b_rooks | b_queen;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t enemy_rooks_and_queens_of() const {
    if constexpr (current_player)
      return b_rooks | b_queen;
    else
      return w_rooks | w_queen;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t bishop_and_queens_of() const {
    if constexpr (current_player)
      return w_bishops | w_queen;
    else
      return b_bishops | b_queen;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t enemy_bishop_and_queens_of() const {
    if constexpr (current_player)
      return b_bishops | b_queen;
    else
      return w_bishops | w_queen;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t OccupiedBy() const {
    if constexpr (current_player)
      return w_occ;
    else
      return b_occ;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t occupied_by_enemy_of() const {
    if constexpr (current_player)
      return b_occ;
    else
      return w_occ;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t Pawns() const {
    if constexpr (current_player)
      return w_pawns;
    else
      return b_pawns;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t enemy_pawns_of() const {
    return this->Pawns<!current_player>();
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t Bishops() const {
    if constexpr (current_player)
      return w_bishops;
    else
      return b_bishops;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t enemy_bishops_of() const {
    return this->Bishops<!current_player>();
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t Knights() const {
    if constexpr (current_player)
      return w_knights;
    else
      return b_knights;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t enemy_knights_of() const {
    return this->Knights<!current_player>();
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t Rooks() const {
    if constexpr (current_player)
      return w_rooks;
    else
      return b_rooks;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t enemy_rooks_of() const {
    return this->Rooks<!current_player>();
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t Queens() const {
    if constexpr (current_player)
      return w_queen;
    else
      return b_queen;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t enemy_queens_of() const {
    return this->Queens<!current_player>();
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t King() const {
    if constexpr (current_player)
      return w_king;
    else
      return b_king;
  }

  template <color_t current_player>
  [[nodiscard]] constexpr inline bitmap_t enemy_king_of() const {
    return this->King<!current_player>();
  }

  [[nodiscard]] Board invertBoard() const {
    return Board(w_pawns, b_pawns, w_bishops, b_bishops, w_knights, b_knights,
                 w_rooks, b_rooks, w_queen, b_queen, w_king, b_king);
  }

  // this is not to most efficient method. SHOULD BE AVOIDED IN MOVE-GEN.
  [[nodiscard]] figure_t get_figure_at(uint8_t tile_index) const;
  [[nodiscard]] figure_t get_figure_at(uint8_t row, uint8_t column) const;

  template <GameState state, figure_type figure, compiletime_move_flag flag>
  Board applyMove(const compiletime_move<figure, flag>& move) const {
    if constexpr (figure == PAWN) {
      return applyPawnMove<state>(move);
    } else if constexpr (figure == BISHOP) {
      return applyBishopMove<state>(move);
    } else if constexpr (figure == KNIGHT) {
      return applyKnightMove<state>(move);
    } else if constexpr (figure == ROOK) {
      return applyRookMove<state>(move);
    } else if constexpr (figure == QUEEN) {
      return applyQueenMove<state>(move);
    } else if constexpr (figure == KING) {
      return applyKingMove<state>(move);
    } else {
      throw std::runtime_error("failed to execute move");
    }
  }

  template <GameState state, compiletime_move_flag flag>
  [[nodiscard]] Board applyPawnMove(
      const compiletime_move<PAWN, flag> move) const {
    if constexpr (state.isWhitesTurn()) {
      return applyPawnMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyPawnMoveBlack(move.m_origin, move.m_target);
    }
  }

  template <GameState state, compiletime_move_flag flag>
  [[nodiscard]] Board applyBishopMove(
      const compiletime_move<BISHOP, flag> move) const {
    if constexpr (state.isWhitesTurn()) {
      return applyBishopMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyBishopMoveBlack(move.m_origin, move.m_target);
    }
  }

  template <GameState state, compiletime_move_flag flag>
  [[nodiscard]] Board applyKnightMove(
      const compiletime_move<KNIGHT, flag> move) const {
    if constexpr (state.isWhitesTurn()) {
      return applyKnightMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyKnightMoveBlack(move.m_origin, move.m_target);
    }
  }

  template <GameState state, compiletime_move_flag flag>
  [[nodiscard]] Board applyRookMove(
      const compiletime_move<ROOK, flag> move) const {
    if constexpr (state.isWhitesTurn()) {
      return applyRookMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyRookMoveBlack(move.m_origin, move.m_target);
    }
  }

  template <GameState state, compiletime_move_flag flag>
  [[nodiscard]] Board applyQueenMove(
      const compiletime_move<QUEEN, flag> move) const {
    if constexpr (state.isWhitesTurn()) {
      return applyQueenMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyQueenMoveBlack(move.m_origin, move.m_target);
    }
  }

  template <GameState state, compiletime_move_flag flag>
  [[nodiscard]] Board applyKingMove(
      const compiletime_move<KING, flag> move) const {
    if constexpr (state.isWhitesTurn()) {
      return applyKingMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyKingMoveBlack(move.m_origin, move.m_target);
    }
  }

  [[nodiscard]] Board applyMove(const GameState& state,
                                const runtime_move& move) {
    switch (move.m_figure) {
      case PAWN:
        return applyPawnMove(state, move);
      case BISHOP:
        return applyBishopMove(state, move);
      case KNIGHT:
        return applyKnightMove(state, move);
      case ROOK:
        return applyRookMove(state, move);
      case QUEEN:
        return applyQueenMove(state, move);
      case KING:
        return applyKingMove(state, move);
      default:
        throw std::runtime_error("failed to execute the move");
    }
  }

  template <GameState state, compiletime_move_flag flag>
  [[nodiscard]] Board applyPawnMove(const compiletime_move<PAWN,
                                                           flag>&
      move) {
    bitmap_t mask = ~move.m_target;
    if constexpr (state.isWhitesTurn()) {
      return applyPawnMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyPawnMoveBlack(move.m_origin, move.m_target);
    }
  }

  [[nodiscard]] Board applyPawnMove(const GameState& state,
                                    const runtime_move& move) {
    assert(move.m_figure == PAWN);
    if (state.isWhitesTurn()) {
      return applyPawnMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyPawnMoveBlack(move.m_origin, move.m_target);
    }
  }

  [[nodiscard]] Board applyBishopMove(const GameState& state,
                                      const runtime_move& move) {
    assert(move.m_figure == BISHOP);
    if (state.isWhitesTurn()) {
      return applyBishopMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyBishopMoveBlack(move.m_origin, move.m_target);
    }
  }

  [[nodiscard]] Board applyKnightMove(const GameState& state,
                                      const runtime_move& move) const {
    assert(move.m_figure == KNIGHT);
    if (state.isWhitesTurn()) {
      return applyKnightMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyKnightMoveBlack(move.m_origin, move.m_target);
    }
  }

  [[nodiscard]] Board applyRookMove(const GameState& state,
                                    const runtime_move& move) const {
    if (state.isWhitesTurn()) {
      return applyRookMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyRookMoveBlack(move.m_origin, move.m_target);
    }
  }

  [[nodiscard]] Board applyQueenMove(const GameState& state,
                                     const runtime_move& move) const {
    if (state.isWhitesTurn()) {
      return applyQueenMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyQueenMoveBlack(move.m_origin, move.m_target);
    }
  }

  [[nodiscard]] Board applyKingMove(const GameState& state,
                                    const runtime_move& move) const {
    if (state.isWhitesTurn()) {
      return applyKingMoveWhite(move.m_origin, move.m_target);
    } else {
      return applyKingMoveBlack(move.m_origin, move.m_target);
    }
  }

 private:
  [[nodiscard]] inline Board applyPawnMoveWhite(bitmap_t origin,
                                                bitmap_t target) const {
    bitmap_t mask = ~target;
    return Board(b_pawns & mask, (w_pawns & (~origin)) | target,
                 b_bishops & mask, w_bishops, b_knights & mask, w_knights,
                 b_rooks & mask, w_rooks, b_queen & mask, w_queen,
                 b_king & mask, w_king);
  }

  [[nodiscard]] inline Board applyPawnMoveBlack(bitmap_t origin,
                                                bitmap_t target) const {
    bitmap_t mask = ~target;
    return Board((b_pawns & (~origin)) | target, w_pawns & mask, b_bishops,
                 w_bishops & mask, b_knights, w_knights & mask, b_rooks,
                 w_rooks & mask, b_queen, w_queen & mask, b_king,
                 w_king & mask);
  }

  [[nodiscard]] inline Board applyBishopMoveWhite(bitmap_t origin,
                                                  bitmap_t target) const {
    bitmap_t mask = ~target;
    return Board(b_pawns & mask, w_pawns, b_bishops & mask,
                 (w_bishops & (~origin)) | target, b_knights & mask, w_knights,
                 b_rooks & mask, w_rooks, b_queen & mask, w_queen,
                 b_king & mask, w_king);
  }

  [[nodiscard]] inline Board applyBishopMoveBlack(bitmap_t origin,
                                                  bitmap_t target) const {
    bitmap_t mask = ~target;
    return Board(b_pawns, w_pawns & mask, (b_bishops & (~origin)) | target,
                 w_bishops & mask, b_knights, w_knights & mask, b_rooks,
                 w_rooks & mask, b_queen, w_queen & mask, b_king,
                 w_king & mask);
  }

  [[nodiscard]] inline Board applyKnightMoveWhite(bitmap_t origin,
                                                  bitmap_t target) const {
    bitmap_t mask = ~target;
    return Board(b_pawns & mask, w_pawns, b_bishops & mask, w_bishops,
                 b_knights & mask, (w_knights & ~origin) | target,
                 b_rooks & mask, w_rooks, b_queen & mask, w_queen,
                 b_king & mask, w_king);
  }

  [[nodiscard]] inline Board applyKnightMoveBlack(bitmap_t origin,
                                                  bitmap_t target) const {
    bitmap_t mask = ~target;
    return Board(b_pawns, w_pawns & mask, b_bishops, w_bishops & mask,
                 (b_knights & ~origin) | target, w_knights & mask, b_rooks,
                 w_rooks & mask, b_queen, w_queen & mask, b_king,
                 w_king & mask);
  }

  [[nodiscard]] inline Board applyRookMoveWhite(bitmap_t origin,
                                                bitmap_t target) const {
    bitmap_t mask = ~target;
    return Board(b_pawns & mask, w_pawns, b_bishops & mask, w_bishops,
                 b_knights & mask, w_knights, b_rooks & mask,
                 (w_rooks & ~origin) | target, b_queen & mask, w_queen,
                 b_king & mask, w_king);
  }

  [[nodiscard]] inline Board applyRookMoveBlack(bitmap_t origin,
                                                bitmap_t target) const {
    bitmap_t mask = ~target;
    return Board(b_pawns, w_pawns & mask, b_bishops, w_bishops & mask,
                 b_knights, w_knights & mask, (b_rooks & ~origin) | target,
                 w_rooks & mask, b_queen, w_queen & mask, b_king,
                 w_king & mask);
  }

  [[nodiscard]] inline Board applyQueenMoveWhite(bitmap_t origin,
                                                 bitmap_t target) const {
    bitmap_t mask = ~target;
    return Board(b_pawns & mask, w_pawns, b_bishops & mask, w_bishops,
                 b_knights & mask, w_knights, b_rooks & mask, w_rooks,
                 b_queen & mask, (w_queen & ~origin) | target, b_king & mask,
                 w_king);
  }

  [[nodiscard]] inline Board applyQueenMoveBlack(bitmap_t origin,
                                                 bitmap_t target) const {
    bitmap_t mask = ~target;
    return Board(b_pawns, w_pawns & mask, b_bishops, w_bishops & mask,
                 b_knights, w_knights & mask, b_rooks, w_rooks & mask,
                 (b_queen & ~origin) | target, w_queen & mask, b_king,
                 w_king & mask);
  }

  [[nodiscard]] inline Board applyKingMoveWhite(bitmap_t origin,
                                                bitmap_t target) const {
    bitmap_t mask = ~target;
    return Board(b_pawns & mask, w_pawns, b_bishops & mask, w_bishops,
                 b_knights & mask, w_knights, b_rooks & mask, w_rooks,
                 b_queen & mask, w_queen, b_king & mask,
                 (w_king & ~origin) | target);
  }

  [[nodiscard]] inline Board applyKingMoveBlack(bitmap_t origin,
                                                bitmap_t target) const {
    bitmap_t mask = ~target;
    return Board(b_pawns, w_pawns & mask, b_bishops, w_bishops & mask,
                 b_knights, w_knights & mask, b_rooks, w_rooks & mask, b_queen,
                 w_queen & mask, (b_king & ~origin) | target, w_king & mask);
  }
};

std::ostream& operator<<(std::ostream& cout, const Board& board);
