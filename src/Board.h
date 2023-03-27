#pragma once
#include <cassert>
#include <cinttypes>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "GameState.h"
#include "bitmap.h"
#include "figure.h"
#include "move.h"

class Board {
 public:
  static constexpr bitmap_t Rank7 = 0xFF000000000000;
  static constexpr bitmap_t Rank8 = 0xFF00000000000000;
  static constexpr bitmap_t Rank1 = 0xFF;
  static constexpr bitmap_t Rank2 = 0xFF00;
  static constexpr bitmap_t File8 = 0x8080808080808080;
  static constexpr bitmap_t File7 = File8 >> 1;
  static constexpr bitmap_t File2 = File7 >> 5;
  static constexpr bitmap_t File1 = File2 >> 1;
  static constexpr bitmap_t WhiteShortCastleMoveMask = 0b01110000ull;
  static constexpr bitmap_t WhiteShortCastleAttackMask = 0b00111000ull;
  static constexpr bitmap_t WhiteShortCastleRookMask = 0b10000000ull;
  static constexpr bitmap_t WhiteLongCastleMoveMask = 0b00000110ull;
  static constexpr bitmap_t WhiteLongCastleAttackMask = 0b00001110ull;
  static constexpr bitmap_t WhiteLongCastleRookMask = 0b00000001ull;
  static constexpr bitmap_t BlackShortCastleMoveMask = 0b01110000ull << 56ull;
  static constexpr bitmap_t BlackShortCastleAttackMask = 0b00111000ull << 56ull;
  static constexpr bitmap_t BlackShortCastleRookMask = 0000000001ull << 56ull;
  static constexpr bitmap_t BlackLongCastleMoveMask = 0b00000110ull << 56ull;
  static constexpr bitmap_t BlackLongCastleAttackMask = 0b00001110ull << 56ull;
  static constexpr bitmap_t BlackLongCastleRookMask = 0b10000000ull << 56ul;

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
  [[nodiscard]] constexpr inline bitmap_t EnemyOrEmpty() const {
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
  [[nodiscard]] constexpr inline bitmap_t EnemyHVSliders() const {
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
  [[nodiscard]] constexpr inline bitmap_t EnemyD12Sliders() const {
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

  template <GameState state, figure figure, move_flag flag>
  force_inline Board
  applyMove(const compiletime_move<figure, flag>& move) const {
    const bitmap_t mov = move.m_origin | move.m_target;
    if constexpr (flag == MOVE_FLAG_CAPTURE || flag == MOVE_FLAG_LEFT_ROOK ||
                  flag == MOVE_FLAG_RIGHT_ROOK) {
      const bitmap_t rem = ~move.m_target;
      if constexpr (state.turn()) {
        assert((b_king & mov) == 0 && "Taking Black King is not legal");
        assert((move.m_target & w_occ) == 0 &&
               "Cannot move to square of same"
               " color");
        if constexpr (figure == PAWN) {
          return Board(b_pawns & rem, w_pawns ^ mov, b_bishops & rem, w_bishops,
                       b_knights & rem, w_knights, b_rooks & rem, w_rooks,
                       b_queen & rem, w_queen, b_king, w_king);
        } else if (figure == KNIGHT) {
          return Board(b_pawns & rem, w_pawns, b_bishops & rem, w_bishops,
                       b_knights & rem, w_knights ^ mov, b_rooks & rem, w_rooks,
                       b_queen & rem, w_queen, b_king, w_king);
        } else if (figure == BISHOP) {
          return Board(b_pawns & rem, w_pawns, b_bishops & rem, w_bishops ^ mov,
                       b_knights & rem, w_knights, b_rooks & rem, w_rooks,
                       b_queen & rem, w_queen, b_king, w_king);
        } else if (figure == ROOK) {
          return Board(b_pawns & rem, w_pawns, b_bishops & rem, w_bishops,
                       b_knights & rem, w_knights, b_rooks & rem, w_rooks ^ mov,
                       b_queen & rem, w_queen, b_king, w_king);
        } else if (figure == QUEEN) {
          return Board(b_pawns & rem, w_pawns, b_bishops & rem, w_bishops,
                       b_knights & rem, w_knights, b_rooks & rem, w_rooks,
                       b_queen & rem, w_queen ^ mov, b_king, w_king);
        } else if (figure == KING) {
          return Board(b_pawns & rem, w_pawns, b_bishops & rem, w_bishops,
                       b_knights & rem, w_knights, b_rooks & rem, w_rooks,
                       b_queen & rem, w_queen, b_king, w_king ^ mov);
        }
      } else {
        assert((w_king & mov) == 0 && "Taking White King is not legal");
        assert((move.m_target & b_occ) == 0 &&
               "Cannot move to square of same"
               " color");
        if constexpr (figure == PAWN) {
          return Board(b_pawns ^ mov, w_pawns & rem, b_bishops, w_bishops & rem,
                       b_knights, w_knights & rem, b_rooks, w_rooks & rem,
                       b_queen, w_queen & rem, b_king, w_king);
        } else if (figure == KNIGHT) {
          return Board(b_pawns, w_pawns & rem, b_bishops, w_bishops & rem,
                       b_knights ^ mov, w_knights & rem, b_rooks, w_rooks & rem,
                       b_queen, w_queen & rem, b_king, w_king);
        } else if (figure == BISHOP) {
          return Board(b_pawns, w_pawns & rem, b_bishops ^ mov, w_bishops & rem,
                       b_knights, w_knights & rem, b_rooks, w_rooks & rem,
                       b_queen, w_queen & rem, b_king, w_king);
        } else if (figure == ROOK) {
          return Board(b_pawns, w_pawns & rem, b_bishops, w_bishops & rem,
                       b_knights, w_knights & rem, b_rooks ^ mov, w_rooks & rem,
                       b_queen, w_queen & rem, b_king, w_king);
        } else if (figure == QUEEN) {
          return Board(b_pawns, w_pawns & rem, b_bishops, w_bishops & rem,
                       b_knights, w_knights & rem, b_rooks, w_rooks & rem,
                       b_queen ^ mov, w_queen & rem, b_king, w_king);
        } else if (figure == KING) {
          return Board(b_pawns, w_pawns & rem, b_bishops, w_bishops & rem,
                       b_knights, w_knights & rem, b_rooks, w_rooks & rem,
                       b_queen, w_queen & rem, b_king ^ mov, w_king);
        }
      }
    } else if (flag == MOVE_FLAG_SHORT_CASTLE) {
      if constexpr (state.turn()) {
        const bitmap_t kingSwitch = 0x50ull;
        const bitmap_t rookSwitch = 0xA0ull;
        return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                     w_knights, b_rooks, w_rooks ^ rookSwitch, b_queen, w_queen,
                     b_king, w_king ^ kingSwitch);
      } else {
        const bitmap_t kingSwitch = 0x50ull << 56ull;
        const bitmap_t rookSwitch = 0xA0ull << 56ull;
        return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                     w_knights, b_rooks ^ rookSwitch, w_rooks, b_queen, w_queen,
                     b_king ^ kingSwitch, w_king);
      }
    } else if (flag == MOVE_FLAG_LONG_CASTLE) {
      if constexpr (state.turn()) {
        const bitmap_t kingSwitch = 0x14ull;
        const bitmap_t rookSwitch = 0x9ull;
        return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                     w_knights, b_rooks, w_rooks ^ rookSwitch, b_queen, w_queen,
                     b_king, w_king ^ kingSwitch);
      } else {
        const bitmap_t kingSwitch = 0x14ull << 56ull;
        const bitmap_t rookSwitch = 0x9ull << 56ull;
        return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                     w_knights, b_rooks ^ rookSwitch, w_rooks, b_queen, w_queen,
                     b_king ^ kingSwitch, w_king);
      }
    } else if constexpr (flag == MOVE_FLAG_EN_PASSANT) {
      if constexpr (state.turn()) {
        const bitmap_t rem = ~(move.m_target >> 8);
        return Board(b_pawns & rem, w_pawns ^ mov, b_bishops & rem, w_bishops,
                     b_knights & rem, w_knights, b_rooks & rem, w_rooks,
                     b_queen & rem, w_queen, b_king, w_king);

      } else {
        const bitmap_t rem = ~(move.m_target << 8);
        return Board(b_pawns ^ mov, w_pawns & rem, b_bishops, w_bishops & rem,
                     b_knights, w_knights & rem, b_rooks, w_rooks & rem,
                     b_queen, w_queen & rem, b_king, w_king);
      }
    } else if constexpr (flag == MOVE_FLAG_PROMOTE) {
      const bitmap_t rem = ~move.m_target;
      if constexpr (state.turn()) {
        if constexpr (figure == KNIGHT) {
          return Board(b_pawns & rem, w_pawns ^ move.m_origin, b_bishops & rem,
                       w_bishops, b_knights & rem, w_knights ^ move.m_target,
                       b_rooks & rem, w_rooks, b_queen & rem, w_queen, b_king,
                       w_king);
        } else if constexpr (figure == BISHOP) {
          return Board(b_pawns & rem, w_pawns ^ move.m_origin, b_bishops & rem,
                       w_bishops ^ move.m_target, b_knights & rem, w_knights,
                       b_rooks & rem, w_rooks, b_queen & rem, w_queen, b_king,
                       w_king);
        } else if constexpr (figure == ROOK) {
          return Board(b_pawns & rem, w_pawns ^ move.m_origin, b_bishops & rem,
                       w_bishops, b_knights & rem, w_knights, b_rooks & rem,
                       w_rooks ^ move.m_target, b_queen & rem, w_queen, b_king,
                       w_king);
        } else if constexpr (figure == QUEEN) {
          return Board(b_pawns & rem, w_pawns ^ move.m_origin, b_bishops & rem,
                       w_bishops, b_knights & rem, w_knights, b_rooks & rem,
                       w_rooks, b_queen & rem, w_queen ^ move.m_target, b_king,
                       w_king);
        }
      } else {
        if constexpr (figure == KNIGHT) {
          return Board(b_pawns ^ move.m_origin, w_pawns & rem, b_bishops,
                       w_bishops & rem, b_knights ^ move.m_target,
                       w_knights & rem, b_rooks, w_rooks & rem, b_queen,
                       w_queen & rem, b_king, w_king);
        } else if constexpr (figure == BISHOP) {
          return Board(b_pawns ^ move.m_origin, w_pawns & rem,
                       b_bishops ^ move.m_target, w_bishops & rem, b_knights,
                       w_knights & rem, b_rooks, w_rooks & rem, b_queen,
                       w_queen & rem, b_king, w_king);
        } else if constexpr (figure == ROOK) {
          return Board(b_pawns ^ move.m_origin, w_pawns & rem, b_bishops,
                       w_bishops & rem, b_knights, w_knights & rem,
                       b_rooks ^ move.m_target, w_rooks & rem, b_queen,
                       w_queen & rem, b_king, w_king);
        } else if constexpr (figure == QUEEN) {
          return Board(b_pawns ^ move.m_origin, w_pawns & rem, b_bishops,
                       w_bishops & rem, b_knights, w_knights & rem, b_rooks,
                       w_rooks & rem, b_queen ^ move.m_target, w_queen & rem,
                       b_king, w_king);
        }
      }

    } else {
      if constexpr (state.turn()) {
        assert((b_king & mov) == 0 && "Taking Black King is not legal");
        assert((move.m_target & w_occ) == 0 &&
               "Cannot move to square of same"
               " color");
        if constexpr (figure == PAWN) {
          return Board(b_pawns, w_pawns ^ mov, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen, b_king,
                       w_king);
        } else if (figure == KNIGHT) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights ^ mov, b_rooks, w_rooks, b_queen, w_queen,
                       b_king, w_king);
        } else if (figure == BISHOP) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops ^ mov, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen, b_king,
                       w_king);
        } else if (figure == ROOK) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks ^ mov, b_queen, w_queen,
                       b_king, w_king);
        } else if (figure == QUEEN) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen ^ mov,
                       b_king, w_king);
        } else if (figure == KING) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen, b_king,
                       w_king ^ mov);
        }
      } else {
        assert((w_king & mov) == 0 && "Taking White King is not legal");
        assert((move.m_target & b_occ) == 0 &&
               "Cannot move to square of same"
               " color");
        if (figure == PAWN) {
          return Board(b_pawns ^ mov, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen, b_king,
                       w_king);
        } else if (figure == KNIGHT) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights ^ mov,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen, b_king,
                       w_king);
        } else if (figure == BISHOP) {
          return Board(b_pawns, w_pawns, b_bishops ^ mov, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen, b_king,
                       w_king);
        } else if (figure == ROOK) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks ^ mov, w_rooks, b_queen, w_queen,
                       b_king, w_king);
        } else if (figure == QUEEN) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen ^ mov, w_queen,
                       b_king, w_king);
        } else if (figure == KING) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen,
                       b_king ^ mov, w_king);
        }
      }
    }
    throw std::runtime_error("Unimplemented Exception");
  }

  [[nodiscard]] Board applyMove(const GameState& state,
                                const runtime_move& move) const {
    const move_flag flag = move.m_flag;
    const figure figure = move.m_figure;
    const bitmap_t mov = move.m_origin | move.m_target;
    if (flag == MOVE_FLAG_CAPTURE || flag == MOVE_FLAG_LEFT_ROOK ||
        flag == MOVE_FLAG_RIGHT_ROOK) {
      const bitmap_t rem = ~move.m_target;
      if (state.turn()) {
        assert((b_king & mov) == 0 && "Taking Black King is not legal");
        assert((move.m_target & w_occ) == 0 &&
               "Cannot move to square of same"
               " color");
        if (figure == PAWN) {
          return Board(b_pawns & rem, w_pawns ^ mov, b_bishops & rem, w_bishops,
                       b_knights & rem, w_knights, b_rooks & rem, w_rooks,
                       b_queen & rem, w_queen, b_king, w_king);
        } else if (figure == KNIGHT) {
          return Board(b_pawns & rem, w_pawns, b_bishops & rem, w_bishops,
                       b_knights & rem, w_knights ^ mov, b_rooks & rem, w_rooks,
                       b_queen & rem, w_queen, b_king, w_king);
        } else if (figure == BISHOP) {
          return Board(b_pawns & rem, w_pawns, b_bishops & rem, w_bishops ^ mov,
                       b_knights & rem, w_knights, b_rooks & rem, w_rooks,
                       b_queen & rem, w_queen, b_king, w_king);
        } else if (figure == ROOK) {
          return Board(b_pawns & rem, w_pawns, b_bishops & rem, w_bishops,
                       b_knights & rem, w_knights, b_rooks & rem, w_rooks ^ mov,
                       b_queen & rem, w_queen, b_king, w_king);
        } else if (figure == QUEEN) {
          return Board(b_pawns & rem, w_pawns, b_bishops & rem, w_bishops,
                       b_knights & rem, w_knights, b_rooks & rem, w_rooks,
                       b_queen & rem, w_queen ^ mov, b_king, w_king);
        } else if (figure == KING) {
          return Board(b_pawns & rem, w_pawns, b_bishops & rem, w_bishops,
                       b_knights & rem, w_knights, b_rooks & rem, w_rooks,
                       b_queen & rem, w_queen, b_king, w_king ^ mov);
        }
      } else {
        assert((w_king & mov) == 0 && "Taking White King is not legal");
        assert((move.m_target & b_occ) == 0 &&
               "Cannot move to square of same"
               " color");
        if (figure == PAWN) {
          return Board(b_pawns ^ mov, w_pawns & rem, b_bishops, w_bishops & rem,
                       b_knights, w_knights & rem, b_rooks, w_rooks & rem,
                       b_queen, w_queen & rem, b_king, w_king);
        } else if (figure == KNIGHT) {
          return Board(b_pawns, w_pawns & rem, b_bishops, w_bishops & rem,
                       b_knights ^ mov, w_knights & rem, b_rooks, w_rooks & rem,
                       b_queen, w_queen & rem, b_king, w_king);
        } else if (figure == BISHOP) {
          return Board(b_pawns, w_pawns & rem, b_bishops ^ mov, w_bishops & rem,
                       b_knights, w_knights & rem, b_rooks, w_rooks & rem,
                       b_queen, w_queen & rem, b_king, w_king);
        } else if (figure == ROOK) {
          return Board(b_pawns, w_pawns & rem, b_bishops, w_bishops & rem,
                       b_knights, w_knights & rem, b_rooks ^ mov, w_rooks & rem,
                       b_queen, w_queen & rem, b_king, w_king);
        } else if (figure == QUEEN) {
          return Board(b_pawns, w_pawns & rem, b_bishops, w_bishops & rem,
                       b_knights, w_knights & rem, b_rooks, w_rooks & rem,
                       b_queen ^ mov, w_queen & rem, b_king, w_king);
        } else if (figure == KING) {
          return Board(b_pawns, w_pawns & rem, b_bishops, w_bishops & rem,
                       b_knights, w_knights & rem, b_rooks, w_rooks & rem,
                       b_queen, w_queen & rem, b_king ^ mov, w_king);
        }
      }
    } else if (flag == MOVE_FLAG_SHORT_CASTLE) {
      if (state.turn()) {
        const bitmap_t kingSwitch = 0x50ull;
        const bitmap_t rookSwitch = 0xA0ull;
        return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                     w_knights, b_rooks, w_rooks ^ rookSwitch, b_queen, w_queen,
                     b_king, w_king ^ kingSwitch);
      } else {
        const bitmap_t kingSwitch = 0x50ull << 56ull;
        const bitmap_t rookSwitch = 0xA0ull << 56ull;
        return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                     w_knights, b_rooks ^ rookSwitch, w_rooks, b_queen, w_queen,
                     b_king ^ kingSwitch, w_king);
      }
    } else if (flag == MOVE_FLAG_LONG_CASTLE) {
      if (state.turn()) {
        const bitmap_t kingSwitch = 0x14ull;
        const bitmap_t rookSwitch = 0x9ull;
        return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                     w_knights, b_rooks, w_rooks ^ rookSwitch, b_queen, w_queen,
                     b_king, w_king ^ kingSwitch);
      } else {
        const bitmap_t kingSwitch = 0x14ull << 56ull;
        const bitmap_t rookSwitch = 0x9ull << 56ull;
        return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                     w_knights, b_rooks ^ rookSwitch, w_rooks, b_queen, w_queen,
                     b_king ^ kingSwitch, w_king);
      }
    } else if (flag == MOVE_FLAG_EN_PASSANT) {
      if (state.turn()) {
        const bitmap_t rem = ~(move.m_target >> 8);
        return Board(b_pawns & rem, w_pawns ^ mov, b_bishops & rem, w_bishops,
                     b_knights & rem, w_knights, b_rooks & rem, w_rooks,
                     b_queen & rem, w_queen, b_king, w_king);

      } else {
        const bitmap_t rem = ~(move.m_target << 8);
        return Board(b_pawns ^ mov, w_pawns & rem, b_bishops, w_bishops & rem,
                     b_knights, w_knights & rem, b_rooks, w_rooks & rem,
                     b_queen, w_queen & rem, b_king, w_king);
      }
    } else if (flag == MOVE_FLAG_PROMOTE) {
      const bitmap_t rem = ~move.m_target;
      if (state.turn()) {
        if (figure == KNIGHT) {
          return Board(b_pawns & rem, w_pawns ^ move.m_origin, b_bishops & rem,
                       w_bishops, b_knights & rem, w_knights ^ move.m_target,
                       b_rooks & rem, w_rooks, b_queen & rem, w_queen, b_king,
                       w_king);
        } else if (figure == BISHOP) {
          return Board(b_pawns & rem, w_pawns ^ move.m_origin, b_bishops & rem,
                       w_bishops ^ move.m_target, b_knights & rem, w_knights,
                       b_rooks & rem, w_rooks, b_queen & rem, w_queen, b_king,
                       w_king);
        } else if (figure == ROOK) {
          return Board(b_pawns & rem, w_pawns ^ move.m_origin, b_bishops & rem,
                       w_bishops, b_knights & rem, w_knights, b_rooks & rem,
                       w_rooks ^ move.m_target, b_queen & rem, w_queen, b_king,
                       w_king);
        } else if (figure == QUEEN) {
          return Board(b_pawns & rem, w_pawns ^ move.m_origin, b_bishops & rem,
                       w_bishops, b_knights & rem, w_knights, b_rooks & rem,
                       w_rooks, b_queen & rem, w_queen ^ move.m_target, b_king,
                       w_king);
        }
      } else {
        if (figure == KNIGHT) {
          return Board(b_pawns ^ move.m_origin, w_pawns & rem, b_bishops,
                       w_bishops & rem, b_knights ^ move.m_target,
                       w_knights & rem, b_rooks, w_rooks & rem, b_queen,
                       w_queen & rem, b_king, w_king);
        } else if (figure == BISHOP) {
          return Board(b_pawns ^ move.m_origin, w_pawns & rem,
                       b_bishops ^ move.m_target, w_bishops & rem, b_knights,
                       w_knights & rem, b_rooks, w_rooks & rem, b_queen,
                       w_queen & rem, b_king, w_king);
        } else if (figure == ROOK) {
          return Board(b_pawns ^ move.m_origin, w_pawns & rem, b_bishops,
                       w_bishops & rem, b_knights, w_knights & rem,
                       b_rooks ^ move.m_target, w_rooks & rem, b_queen,
                       w_queen & rem, b_king, w_king);
        } else if (figure == QUEEN) {
          return Board(b_pawns ^ move.m_origin, w_pawns & rem, b_bishops,
                       w_bishops & rem, b_knights, w_knights & rem, b_rooks,
                       w_rooks & rem, b_queen ^ move.m_target, w_queen & rem,
                       b_king, w_king);
        }
      }

    } else {
      if (state.turn()) {
        assert((b_king & mov) == 0 && "Taking Black King is not legal");
        assert((move.m_target & w_occ) == 0 &&
               "Cannot move to square of same"
               " color");
        if (figure == PAWN) {
          return Board(b_pawns, w_pawns ^ mov, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen, b_king,
                       w_king);
        } else if (figure == KNIGHT) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights ^ mov, b_rooks, w_rooks, b_queen, w_queen,
                       b_king, w_king);
        } else if (figure == BISHOP) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops ^ mov, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen, b_king,
                       w_king);
        } else if (figure == ROOK) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks ^ mov, b_queen, w_queen,
                       b_king, w_king);
        } else if (figure == QUEEN) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen ^ mov,
                       b_king, w_king);
        } else if (figure == KING) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen, b_king,
                       w_king ^ mov);
        }
      } else {
        assert((w_king & mov) == 0 && "Taking White King is not legal");
        assert((move.m_target & b_occ) == 0 &&
               "Cannot move to square of same"
               " color");
        if (figure == PAWN) {
          return Board(b_pawns ^ mov, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen, b_king,
                       w_king);
        } else if (figure == KNIGHT) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights ^ mov,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen, b_king,
                       w_king);
        } else if (figure == BISHOP) {
          return Board(b_pawns, w_pawns, b_bishops ^ mov, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen, b_king,
                       w_king);
        } else if (figure == ROOK) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks ^ mov, w_rooks, b_queen, w_queen,
                       b_king, w_king);
        } else if (figure == QUEEN) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen ^ mov, w_queen,
                       b_king, w_king);
        } else if (figure == KING) {
          return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights,
                       w_knights, b_rooks, w_rooks, b_queen, w_queen,
                       b_king ^ mov, w_king);
        }
      }
    }
    throw std::runtime_error("Unimplemented Exception");
  }
};

std::ostream& operator<<(std::ostream& cout, const Board& board);
