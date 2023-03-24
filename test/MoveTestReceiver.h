#pragma once
#include "MoveBitmapAccumulator.h"
#include "MoveCounter.h"
#include "bitmap.h"
#include "figure.h"
#include "move.h"

class MoveTestReceiver {
 private:
  MoveCounter m_counterReceiver;
  MoveBitmapAccumulator m_bitmapAccumulator;

 public:
  template <figure_type type, compiletime_move_flag cflag>
  inline void move(bitmap_t origin, bitmap_t target, move_flag flag) {
    m_counterReceiver.move<type>(origin, target, flag);
    m_bitmapAccumulator.move<type>(origin, target, flag);
  }

  [[nodiscard]] unsigned long long moveCount() const {
    return m_counterReceiver.totalCount();
  }

  [[nodiscard]] unsigned long long pawnMoveCount() const {
    return m_counterReceiver.pawnCount();
  }

  [[nodiscard]] unsigned long long bishopMoveCount() const {
    return m_counterReceiver.bishopCount();
  }

  [[nodiscard]] unsigned long long knightMoveCount() const {
    return m_counterReceiver.knightCount();
  }

  [[nodiscard]] unsigned long long rookMoveCount() const {
    return m_counterReceiver.rookCount();
  }

  [[nodiscard]] unsigned long long queenMoveCount() const {
    return m_counterReceiver.queenCount();
  }

  [[nodiscard]] unsigned long long kingMoveCount() const {
    return m_counterReceiver.kingCount();
  }

  [[nodiscard]] bitmap_t origins() const {
    return m_bitmapAccumulator.origins();
  }
  [[nodiscard]] bitmap_t targets() const {
    return m_bitmapAccumulator.targets();
  }
  [[nodiscard]] bitmap_t pawnOrigins() const {
    return m_bitmapAccumulator.pawnOrigins();
  }
  [[nodiscard]] bitmap_t pawnTargets() const {
    return m_bitmapAccumulator.pawnTargets();
  }
  [[nodiscard]] bitmap_t bishopOrigins() const {
    return m_bitmapAccumulator.bishopOrigins();
  }
  [[nodiscard]] bitmap_t bishopTargets() const {
    return m_bitmapAccumulator.bishopTargets();
  }
  [[nodiscard]] bitmap_t knightOrigins() const {
    return m_bitmapAccumulator.knightOrigins();
  }
  [[nodiscard]] bitmap_t knightTargets() const {
    return m_bitmapAccumulator.knightTargets();
  }
  [[nodiscard]] bitmap_t rookOrigins() const {
    return m_bitmapAccumulator.rookOrigins();
  }
  [[nodiscard]] bitmap_t rookTargets() const {
    return m_bitmapAccumulator.rookTargets();
  }
  [[nodiscard]] bitmap_t queenOrigins() const {
    return m_bitmapAccumulator.queenOrigins();
  }
  [[nodiscard]] bitmap_t queenTargets() const {
    return m_bitmapAccumulator.queenTargets();
  }
  [[nodiscard]] bitmap_t kingOrigins() const {
    return m_bitmapAccumulator.kingOrigins();
  }
  [[nodiscard]] bitmap_t kingTargets() const {
    return m_bitmapAccumulator.kingTargets();
  }

  void reset() {
    m_counterReceiver.reset();
    m_bitmapAccumulator.reset();
  }
};