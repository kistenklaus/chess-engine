#include "figure.h"

figure_t::figure_t() : bits(0) {}
figure_t::figure_t(uint8_t bits) : bits(bits) {}
figure_t::figure_t(color_t color, figure_type figure_type)
    : bits(color | figure_type) {}

std::ostream &operator<<(std::ostream &cout, const figure_t &figure) {
  figure_type type = figure.type();
  switch (type) {
    case PAWN:
      cout << "p";
      break;
    case BISHOP:
      cout << "b";
      break;
    case KNIGHT:
      cout << "k";
      break;
    case ROOK:
      cout << "r";
      break;
    case QUEEN:
      cout << "Q";
      break;
    case KING:
      cout << "K";
      break;
    default:
      break;
  }
  return cout;
}
