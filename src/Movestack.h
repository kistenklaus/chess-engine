#pragma once

#include <cassert>
#include <cinttypes>
#include <vector>

#include "Board.h"
#include "bitmap.h"
#include "figure.h"
#include "move.h"

class MovestackFrame {
 private:
  Board m_board;
  std::vector<move_t<PAWN>> m_pawnMoves;
  std::vector<move_t<BISHOP>> m_bishopMoves;
  std::vector<move_t<KNIGHT>> m_knightMoves;
  std::vector<move_t<ROOK>> m_rookMoves;
  std::vector<move_t<QUEEN>> m_queenMoves;
  std::vector<move_t<KING>> m_kingMoves;

 public:
  explicit MovestackFrame(const Board& board) : m_board(board) {}

  explicit MovestackFrame(const Board&& board) : m_board(board) {}

  template <figure_type type>
  inline void submit(bitmap_t originBitmap, bitmap_t targetBitmap,
                     uint32_t flags) {
    static_assert(type != NOT_A_FIGURE);
    if constexpr (type == PAWN) {
      m_pawnMoves.emplace_back(originBitmap, targetBitmap, flags);
    } else if constexpr (type == BISHOP) {
      m_bishopMoves.emplace_back(originBitmap, targetBitmap, flags);
    } else if constexpr (type == KNIGHT) {
      m_knightMoves.emplace_back(originBitmap, targetBitmap, flags);
    } else if constexpr (type == ROOK) {
      m_rookMoves.emplace_back(originBitmap, targetBitmap, flags);
    } else if constexpr (type == QUEEN) {
      m_queenMoves.emplace_back(originBitmap, targetBitmap, flags);
    } else if constexpr (type == KING) {
      m_kingMoves.emplace_back(originBitmap, targetBitmap, flags);
    }
  }

  inline unsigned long moveCount() {
    return m_pawnMoves.size() + m_bishopMoves.size() + m_knightMoves.size() +
           m_rookMoves.size() + m_queenMoves.size() + m_kingMoves.size();
  }

  inline void reset() {
    m_pawnMoves.clear();
    m_bishopMoves.clear();
    m_knightMoves.clear();
    m_rookMoves.clear();
    m_queenMoves.clear();
    m_kingMoves.clear();
  }
};

class Movestack {
 private:
  std::vector<MovestackFrame> m_stack;

 public:
  inline MovestackFrame& peek() { return m_stack.back(); }

  inline MovestackFrame& getFrame(unsigned int depth) {
    assert(depth < m_stack.size());
    return m_stack[depth];
  }

  inline void push(const Board& board) { m_stack.emplace_back(board); }

  inline void pop() { m_stack.pop_back(); }

  template <figure_type type>
  inline void submit(bitmap_t originBitmap, bitmap_t targetBitmap,
                     uint32_t flags) {
    m_stack.back().submit<type>(originBitmap, targetBitmap, flags);
  }
};