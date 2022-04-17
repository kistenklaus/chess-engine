#include <iostream>

#include "Board.h"
#include "fen.h"

int main() {
  Board board = board_from_fen("");
  std::cout << board << std::endl;
}
