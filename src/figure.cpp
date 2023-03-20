#include "figure.h"

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
std::ostream &operator<<(std::ostream &cout, const figure_type &figure_type) {
  switch (figure_type) {
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
