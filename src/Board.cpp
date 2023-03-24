#include "Board.h"

#include <cassert>

#include "tile_index.h"

figure_t Board::get_figure_at(uint8_t tile_index) const {
  assert(tile_index <= 63);
  bitmap_t tile_mask = (((bitmap_t)0x1ULL) << tile_index);
  if (tile_mask & Occupied()) {
    if (tile_mask & OccupiedBy<WHITE>()) {
      if (tile_mask & Pawns<WHITE>()) {
        return figure_t(WHITE, PAWN);
      } else if (tile_mask & Bishops<WHITE>()) {
        return figure_t(WHITE, BISHOP);
      } else if (tile_mask & Knights<WHITE>()) {
        return figure_t(WHITE, KNIGHT);
      } else if (tile_mask & Rooks<WHITE>()) {
        return figure_t(WHITE, ROOK);
      } else if (tile_mask & Queens<WHITE>()) {
        return figure_t(WHITE, QUEEN);
      } else if (tile_mask & King<WHITE>()) {
        return figure_t(WHITE, KING);
      }
    } else if (tile_mask & OccupiedBy<BLACK>()) {
      if (tile_mask & Pawns<BLACK>()) {
        return figure_t(BLACK, PAWN);
      } else if (tile_mask & Bishops<BLACK>()) {
        return figure_t(BLACK, BISHOP);
      } else if (tile_mask & Knights<BLACK>()) {
        return figure_t(BLACK, KNIGHT);
      } else if (tile_mask & Rooks<BLACK>()) {
        return figure_t(BLACK, ROOK);
      } else if (tile_mask & Queens<BLACK>()) {
        return figure_t(BLACK, QUEEN);
      } else if (tile_mask & King<BLACK>()) {
        return figure_t(BLACK, KING);
      }
    }
  }
  return figure_t();  // equivilent to figure_t(BLACK, EMPTY); [bits == 0]
}

figure_t Board::get_figure_at(uint8_t row, uint8_t column) const {
  return get_figure_at(row * 8 + column);
}

/// Basicly toString for std::cout
std::ostream &operator<<(std::ostream &cout, const Board &board) {
  cout << std::endl;
  for (int row = 7; row >= 0; row--) {
    cout << "  ";
    for (int column = 0; column < 8; column++) {
      uint8_t tile_index = row * 8 + column;
      const figure_t figure = board.get_figure_at(tile_index);
      std::stringstream stream;
      stream << figure << " ";;
      std::string occ = stream.str();
      size_t length = occ.length();
      if (length < 2) {
        occ.insert(length, 2 - length, ' ');
      }
      if (figure.color() == WHITE) {
        cout << "\x1B[31m";
      } else {  // color() == BLACK
        cout << "\x1B[30m";
      }
      if ((column + row * 9) % 2 == 1) {
        cout << "\x1B[43m" << occ << "\033[0m";
      } else {
        cout << "\x1B[44m" << occ << "\033[0m";
      }
      cout << "\033[0m";
    }
    cout << std::endl;
  }
  return cout;
}
