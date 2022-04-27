#include "fen.h"

#include <assert.h>

#include <iostream>
#include <string>

Board fen::parse(const std::string FEN) {

  std::string split[6];
  int index = 0;
  for (const char c : FEN) {
    if (c == ' ') {
      index++;
      continue;
    }
    split[index] += c;
  }
  const std::string brd_str = split[0];
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
  bitmap_t w_king = 0;
  std::string rank_fens[8];
  int i = 0;
  for (char c : brd_str) {
    if (c == '/') {
      i++;
      continue;
    }
    rank_fens[7 - i] += c;
  }
  for (int rank = 0; rank < 8; rank++) {
    int file = 0;
    for (char c : rank_fens[rank]) {
      if (c >= '0' && c <= '9') {
        int delta = c - '0';
        file += delta;
        continue;
      }
      const bitmap_t mask = ((bitmap_t)1) << (rank * 8 + file);
      switch (c) {
      case 'K':
        w_king |= mask;
        break;
      case 'k':
        b_king |= mask;
        break;
      case 'Q':
        w_queen |= mask;
        break;
      case 'q':
        b_queen |= mask;
        break;
      case 'R':
        w_rooks |= mask;
        break;
      case 'r':
        b_rooks |= mask;
        break;
      case 'N':
        w_knights |= mask;
        break;
      case 'n':
        b_knights |= mask;
        break;
      case 'B':
        w_bishops |= mask;
        break;
      case 'b':
        b_bishops |= mask;
        break;
      case 'P':
        w_pawns |= mask;
        break;
      case 'p':
        b_pawns |= mask;
        break;
      }
      file++;
    }
  }

  const std::string turn_str = split[1];
  const bool white_turn = turn_str == "w";
  assert(white_turn || turn_str == "b");

  const std::string rochade_str = split[2];
  const bool white_short_castle = rochade_str.find('K') != std::string::npos;
  const bool black_short_castle = rochade_str.find('k') != std::string::npos;
  const bool white_long_castle = rochade_str.find('Q') != std::string::npos;
  const bool black_long_castle = rochade_str.find('q') != std::string::npos;

  const std::string en_passant_str = split[3];
  const bool has_en_passant = en_passant_str != "-";
  if (has_en_passant) {
    if (en_passant_str.size() == 2) {
      std::cerr << "INVALID FEN ENPASSANT" << std::endl;
    }
    const int file = en_passant_str[0] - 'a';
    const int rank = en_passant_str[1] - '0';
  }

  const std::string half_turns_str = split[4];
  const int half_turns = std::stoi(half_turns_str);

  const std::string next_turn_str = split[5];
  const int next_turn = std::stoi(next_turn_str);

  return Board(b_pawns, w_pawns, b_bishops, w_bishops, b_knights, w_knights,
               b_rooks, w_rooks, b_queen, w_queen, b_king, w_king);
}
