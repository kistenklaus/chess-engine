#include "figure.h"

figure_t::figure_t() : bits(0) {}
figure_t::figure_t(uint8_t bits) : bits(bits) {}
figure_t::figure_t(color_t color, figure_type figure_type)
    : bits(color | figure_type) {}

std::ostream &operator<<(std::ostream &cout, const figure_t &figure) {
  figure_type type = figure.type();
  switch (type) {
    case PAWN:
      cout << "";
      break;
    case BISHOP:
      cout << "";
      break;
    case KNIGHT:
      cout << "";
      break;
    case ROOK:
      cout << "";
      break;
    case QUEEN:
      cout << "";
      break;
    case KING:
      cout << "";
      break;
    default:
      break;
  }
  return cout;
}
