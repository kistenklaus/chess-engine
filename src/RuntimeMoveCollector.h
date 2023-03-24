//
// Created by karl on 22/03/23.
//
#pragma once

#include <vector>

#include "bitmap.h"
#include "figure.h"
#include "move.h"

class RuntimeMoveCollector {
 public:
  template <figure_type figure, compiletime_move_flag cflag>
  void move(bitmap_t origin, bitmap_t target, move_flag flag) {
    runtime_move move{.m_origin = origin,
                      .m_target = target,
                      .m_figure = figure,
                      .m_flags = flag};
    m_moves.push_back(move);
  }
  [[nodiscard]] unsigned int count() const { return m_moves.size(); }
  [[nodiscard]] const std::vector<runtime_move>& moves() const {
    return m_moves;
  }

  void reset() {
    m_moves.clear();
  }

 private:
  std::vector<runtime_move> m_moves;
};
