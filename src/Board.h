#pragma once
#include <iostream>
#include <sstream>

#include "bitmap.h"
#include "cinttypes"
#include "figure.h"

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
  const bitmap_t b_occ;
  const bitmap_t w_occ;
  const bitmap_t occ;
  const bitmap_t b_pawns;
  const bitmap_t w_pawns;
  const bitmap_t b_bishops;
  const bitmap_t w_bishops;
  const bitmap_t b_knights;
  const bitmap_t w_knights;
  const bitmap_t b_rooks;
  const bitmap_t w_rooks;
  const bitmap_t w_king;
  const bitmap_t b_king;
  const bitmap_t w_queen;
  const bitmap_t b_queen;

 public:
  Board(bitmap_t b_pawns, bitmap_t w_pawns, bitmap_t b_bishops,
        bitmap_t w_bishops, bitmap_t b_knights, bitmap_t w_knights,
        bitmap_t b_rooks, bitmap_t w_rooks, bitmap_t b_queen, bitmap_t w_queen,
        bitmap_t b_king, bitmap_t w_king)
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

  constexpr inline bitmap_t occupied() const { return occ; }

  constexpr inline bitmap_t not_occupied() const { return ~occ; }

  template <color_t current_player>
  constexpr inline bitmap_t empty_or_occupied_by_enemy_of() const {
    if constexpr (current_player)
      return ~w_occ;
    else
      return ~b_occ;
  }

  template <color_t current_player>
  constexpr inline bitmap_t rooks_and_queens_of() const {
    if constexpr (current_player)
      return w_rooks | w_queen;
    else
      return b_rooks | b_queen;
  }

  template <color_t current_player>
  constexpr inline bitmap_t enemy_rooks_and_queens_of() const {
    if constexpr (current_player)
      return b_rooks | b_queen;
    else
      return w_rooks | w_queen;
  }

  template <color_t current_player>
  constexpr inline bitmap_t bishop_and_queens_of() const {
    if constexpr (current_player)
      return w_bishops | w_queen;
    else
      return b_bishops | b_queen;
  }

  template <color_t current_player>
  constexpr inline bitmap_t enemy_bishop_and_queens_of() const {
    if constexpr (current_player)
      return b_bishops | b_queen;
    else
      return w_bishops | w_queen;
  }

  template <color_t current_player>
  constexpr inline bitmap_t occupied_by() const {
    if constexpr (current_player)
      return w_occ;
    else
      return b_occ;
  }

  template <color_t current_player>
  constexpr inline bitmap_t occupied_by_enemy_of() const {
    if constexpr (current_player)
      return b_occ;
    else
      return w_occ;
  }

  template <bool current_player>
  constexpr inline bitmap_t pawns_of() const {
    if constexpr (current_player)
      return w_pawns;
    else
      return b_pawns;
  }

  template <bool current_player>
  constexpr inline bitmap_t enemy_pawns_of() const {
    return this->pawns_of<!current_player>();
  }

  template <bool current_player>
  constexpr inline bitmap_t bishops_of() const {
    if constexpr (current_player)
      return w_bishops;
    else
      return b_bishops;
  }

  template <bool current_player>
  constexpr inline bitmap_t enemy_bishops_of() const {
    return this->bishops_of<!current_player>();
  }

  template <bool current_player>
  constexpr inline bitmap_t knights_of() const {
    if constexpr (current_player)
      return w_knights;
    else
      return b_knights;
  }

  template <bool current_player>
  constexpr inline bitmap_t enemy_knights_of() const {
    return this->knights_of<!current_player>();
  }

  template <bool current_player>
  constexpr inline bitmap_t rooks_of() const {
    if constexpr (current_player)
      return w_rooks;
    else
      return b_rooks;
  }

  template <bool current_player>
  constexpr inline bitmap_t enemy_rooks_of() const {
    return this->rooks_of<!current_player>();
  }

  template <bool current_player>
  constexpr inline bitmap_t queens_of() const {
    if constexpr (current_player)
      return w_queen;
    else
      return b_queen;
  }

  template <bool current_player>
  constexpr inline bitmap_t enemy_queens_of() const {
    return this->queens_of<!current_player>();
  }

  template <bool current_player>
  constexpr inline bitmap_t king_of() const {
    if constexpr (current_player)
      return w_king;
    else
      return b_king;
  }

  template <bool current_player>
  constexpr inline bitmap_t enemy_king_of() const {
    return this->king_of<!current_player>();
  }

  // this is not to most efficient method. SHOULD AVOIDED IN MOVEGEN.
  figure_t get_figure_at(uint8_t tile_index) const;
  figure_t get_figure_at(uint8_t row, uint8_t column) const;
};

std::ostream &operator<<(std::ostream &cout, const Board &board);
