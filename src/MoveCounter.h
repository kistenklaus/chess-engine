#pragma once

#include <iostream>

#include "bitmap.h"
#include "figure.h"
#include "move.h"

class MoveCounter {
 private:
  unsigned long long m_pawnMoves = 0;
  unsigned long long m_bishopMoves = 0;
  unsigned long long m_knightMoves = 0;
  unsigned long long m_rookMoves = 0;
  unsigned long long m_queenMoves = 0;
  unsigned long long m_kingMoves = 0;

 public:
  template <figure_type type>
  inline void move(bitmap_t origin, bitmap_t target, move_flag flag) {
    if constexpr (type == PAWN) {
      m_pawnMoves += 1;
    } else if constexpr (type == BISHOP) {
      m_bishopMoves += 1;
    } else if constexpr (type == KNIGHT) {
      m_knightMoves += 1;
    } else if constexpr (type == ROOK) {
      m_rookMoves += 1;
    } else if constexpr (type == QUEEN) {
      m_queenMoves += 1;
    } else if constexpr (type == KING) {
      m_kingMoves += 1;
    }
  }

  [[nodiscard]] unsigned long long totalCount() const{
    return m_pawnMoves + m_bishopMoves + m_knightMoves + m_rookMoves +
           m_queenMoves + m_kingMoves;
  }
  [[nodiscard]] unsigned long long pawnCount() const{
    return m_pawnMoves;
  }
  [[nodiscard]] unsigned long long bishopCount()const {
    return m_bishopMoves;
  }
  [[nodiscard]] unsigned long long knightCount()const{
    return m_knightMoves;
  }
  [[nodiscard]] unsigned long long rookCount() const {
    return m_rookMoves;
  }
  [[nodiscard]] unsigned long long queenCount() const {
    return m_queenMoves;
  }
  [[nodiscard]] unsigned long long kingCount() const {
    return m_kingMoves;
  }

  void reset(){
    m_pawnMoves = 0;
    m_bishopMoves = 0;
    m_knightMoves = 0;
    m_rookMoves = 0;
    m_queenMoves = 0;
    m_kingMoves = 0;
  }
};