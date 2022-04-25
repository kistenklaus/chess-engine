#pragma once

#include "Board.h"
#include "bitmap.h"

Board board_from_fen(const std::string FEN) {
  bitmap_t b_pawns = 0;
  bitmap_t b_bishops = 0;
  bitmap_t b_knights = 0;
  bitmap_t b_rooks = 0;
  bitmap_t b_queen = 0;
  bitmap_t b_king = 0;
  bitmap_t w_pawns = 0;
  bitmap_t w_bishops = 0;
  bitmap_t w_knights = 0;
  bitmap_t w_rooks = 0;
  bitmap_t w_queen = 0;
  bitmap_t w_king = 1 << 3;
  // TODO implement fen parsing !!!

  return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights, w_knights,
               b_rooks, w_rooks, b_queen, w_queen, b_king, w_king);
}
