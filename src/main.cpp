#include <iostream>

#include "Board.h"
#include "KnightLookUpTable.h"
#include "fen.h"

int main() {
  Board board = board_from_fen("not yet implemented");
  std::cout << board << std::endl;

  KnightLookUpTable::get()[10];
}
