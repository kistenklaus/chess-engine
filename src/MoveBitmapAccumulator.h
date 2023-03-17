#pragma once
#include <cinttypes>

#include "bitmap.h"
#include "figure.h"
#include "move.h"

class MoveBitmapAccumulator {
 private:
  bitmap_t m_pawnOrigins = 0;
  bitmap_t m_pawnTargets = 0;
  bitmap_t m_bishopOrigins = 0;
  bitmap_t m_bishopTargets = 0;
  bitmap_t m_knightOrigins = 0;
  bitmap_t m_knightTargets = 0;
  bitmap_t m_rookOrigins = 0;
  bitmap_t m_rookTargets = 0;
  bitmap_t m_queenOrigins = 0;
  bitmap_t m_queenTargets = 0;
  bitmap_t m_kingOrigins = 0;
  bitmap_t m_kingTargets = 0;

 public:
  template <figure_type type>
  inline void move(bitmap_t origin, bitmap_t target, move_flag flag) {
    static_assert(type != NOT_A_FIGURE);
    if constexpr (type == PAWN) {
      m_pawnOrigins |= origin;
      m_pawnTargets |= target;
    } else if constexpr (type == BISHOP) {
      m_bishopOrigins |= origin;
      m_bishopTargets |= target;
    } else if constexpr (type == KNIGHT) {
      m_knightOrigins |= origin;
      m_knightTargets |= target;
    } else if constexpr (type == ROOK) {
      m_rookOrigins |= origin;
      m_rookTargets |= target;
    } else if constexpr (type == QUEEN) {
      m_queenOrigins |= origin;
      m_queenTargets |= target;
    }else if constexpr (type == KING){
      m_kingOrigins |= origin;
      m_kingTargets |= target;
    }
  }

  [[nodiscard]] bitmap_t origins() const {
    return m_pawnOrigins | m_bishopOrigins | m_knightOrigins | m_rookOrigins |
           m_queenOrigins | m_kingOrigins;
  }
  [[nodiscard]] bitmap_t targets() const {
    return m_pawnTargets | m_bishopTargets | m_knightTargets | m_rookTargets |
           m_queenTargets | m_kingTargets;
  }

  [[nodiscard]] bitmap_t pawnOrigins() const { return m_pawnOrigins; }
  [[nodiscard]] bitmap_t pawnTargets() const { return m_pawnTargets; }
  [[nodiscard]] bitmap_t bishopOrigins() const { return m_bishopOrigins; }
  [[nodiscard]] bitmap_t bishopTargets() const { return m_bishopTargets; }
  [[nodiscard]] bitmap_t knightOrigins() const { return m_knightOrigins; }
  [[nodiscard]] bitmap_t knightTargets() const { return m_knightTargets; }
  [[nodiscard]] bitmap_t rookOrigins() const { return m_rookOrigins; }
  [[nodiscard]] bitmap_t rookTargets() const { return m_rookTargets; }
  [[nodiscard]] bitmap_t queenOrigins() const { return m_queenOrigins; }
  [[nodiscard]] bitmap_t queenTargets() const { return m_queenTargets; }
  [[nodiscard]] bitmap_t kingOrigins() const { return m_kingOrigins; }
  [[nodiscard]] bitmap_t kingTargets() const { return m_kingTargets; }

  void reset() {
    m_pawnOrigins = 0;
    m_pawnTargets = 0;
    m_bishopOrigins = 0;
    m_bishopTargets = 0;
    m_knightOrigins = 0;
    m_knightTargets = 0;
    m_rookOrigins = 0;
    m_rookTargets = 0;
    m_queenOrigins = 0;
    m_queenTargets = 0;
    m_kingOrigins = 0;
    m_kingTargets = 0;
  }
};