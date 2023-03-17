#include <iostream>

#include "MoveCounter.h"
#include "MoveBitmapAccumulator.h"
#include "Board.h"
#include "BoardState.h"
#include "build_config.h"
#include "display.h"
#include "fen.h"
#include "lookup.h"
#include "move_generation.h"
#include "x86utils.h"

int main() {
  // const Board board = fen::parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
  // w KQkq - 0 1");
  //  const Board board = fen::parse("8/3p1p2/8/4K3/1p4p1/8/8/8 w - - 0 1");
  //  const Board board = fen::parse("8/8/8/3Q4/8/8/8/1K4k1 w - - 0 1");
  //  const Board board = fen::parse("8/6k1/8/8/4n3/2K5/8/8 w - - 0 1");
  //  //knight checkmask test. const Board board =
  //  fen::parse("8/6k1/8/8/3p4/2K5/8/8 w - - 0 1"); //pawn checkmask test #1.
  //  const Board board = fen::parse("8/6k1/8/8/1p6/2K5/8/8 w - - 0 1"); //pawn
  //  checkmask test #2. const Board board = fen::parse("8/6k1/8/4b3/8/8/1K6/8 b
  //  - - 0 1"); //bishop checkmask test #1.

  // bishop checkmask down right:
  // const Board board = fen::parse("8/6k1/2b5/8/8/8/6K1/8 b - - 0 1"); //bishop
  // checkmask test #2. const Board board = fen::parse("8/6k1/2b5/8/4P3/8/6K1/8
  // b - - 0 1"); //bishop checkmask test #3. const Board board =
  // fen::parse("8/6k1/2b5/8/4p3/8/6K1/8 b - - 0 1"); //bishop checkmask test
  // #4.

  // bishop checkmask down left.
  // const Board board = fen::parse("8/1k6/8/4b3/8/8/1K6/8 w - - 0 1"); //bishop
  // checkmask test #5. const Board board = fen::parse("8/1k4b1/8/8/3p4/8/1K6/8
  // w - - 0 1"); //bishop checkmask test #6.

  // bishop checmask top right
  // const Board board = fen::parse("3k3K/8/8/8/8/8/8/b7 w - - 0 1"); //bishop
  // checkmask test #7. const Board board = fen::parse("3k3K/8/8/4P3/8/8/8/b7 w
  // - - 0 1"); //bishop checkmask test #8.

  // rook checkmask down.
  // const Board board = fen::parse("8/3r4/k7/8/8/8/3K4/8 b - - 0 1"); //rook
  // checkmask test #1. const Board board = fen::parse("8/3r4/k7/3P4/8/8/3K4/8 b
  // - - 0 1"); //rook checkmask test #2.

  // rook checkmask left
  // const Board board = fen::parse("8/8/8/K3P1r1/8/8/2k5/8 b - - 0 1"); //rook

  const Board board = fen::parse("2k5/8/K7/8/4Q3/8/8/8 w - - 0 1");

  //const move_t move = move_t(0, 0, PAWN, move_t::FLAG_NONE);
  //const Board nextBoard = board.makeMove<BoardState::Default()>(move);

  // checkmask test #3. const Board board = fen::parse("8/8/8/K5r1/8/8/2k5/8 b -
  // - 0 1"); //rook checkmask test #4.

  // rook pinmask left.
  // const Board board = fen::parse("8/3q4/8/8/2KRP1r1/8/8/6k1 b - - 0 1");
  // //rook pinmask test #1.
  //
  // king moves check.
  // const Board board = fen::parse("8/3q4/8/8/2K5/8/8/6k1 b - - 0 1"); //king
  // move test #1. const Board board = fen::parse("8/1P1q4/8/8/2K5/8/8/6k1 b - -
  // 0 1"); //king move test #2.

  // const Board board = fen::parse("8/5q2/8/8/2P5/1K3k2/8/8 w - - 0 1"); //pawn
  //  pin test #1.
  print_display(board);
  // std::cout << board << std::endl;

  MoveCounter counter;
  MoveBitmapAccumulator receiver;

  generate_moves<BoardState::Default()>(board, receiver);
}
