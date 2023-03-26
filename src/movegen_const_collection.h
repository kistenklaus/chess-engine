//
// Created by karl on 26/03/23.
//
#include "movegen_constant_enumeration.h"

namespace movegen::collection {

class MoveVectorCollector {
 public:
  explicit MoveVectorCollector(std::vector<runtime_move>& moves)
      : m_moves(moves) {}
  template <figure_type figure, move_flag flag>
  force_inline void move(bitmap_t origin, bitmap_t target) {
    m_moves.push_back(runtime_move{.m_origin = origin,
                                   .m_target = target,
                                   .m_figure = figure,
                                   .m_flag = flag});
  }

 private:
  std::vector<runtime_move>& m_moves;
};

class MoveCollector {
 public:
  explicit MoveCollector(runtime_move* moves) : m_moves(moves) {}

  template <figure_type figure, move_flag flag>
  force_inline void move(bitmap_t origin, bitmap_t target) {
    *m_moves++ = runtime_move{.m_origin = origin,
                              .m_target = target,
                              .m_figure = figure,
                              .m_flag = flag};
    m_count++;
  }
  [[nodiscard]] unsigned int count() const { return m_count; }

 private:
  runtime_move* m_moves;
  unsigned int m_count = 0;
};

}  // namespace movegen::collection

namespace movegen {
template <GameState state, int depth>
never_inline void collect(const Board& board,
                          std::vector<runtime_move>& moves) {
  collection::MoveVectorCollector collector(moves);
  movegen::enumerate<state, depth, collection::MoveVectorCollector>(board,
                                                                    collector);
}

template <GameState state, int depth>
never_inline unsigned int collect(const Board& board, runtime_move* moves) {
  collection::MoveCollector collector(moves);
  movegen::enumerate<state, depth, collection::MoveVectorCollector>(board,
                                                                    collector);
  return collector.count();
}

}  // namespace movegen
