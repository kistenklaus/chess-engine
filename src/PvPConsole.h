#pragma once

#include <algorithm>
#include <iostream>
#include <string>

#include "Board.h"
#include "RuntimeMoveCollector.h"
#include "algebraic_notation.h"
#include "display.h"
#include "fen.h"
#include "move.h"
#include "move_generation.h"

class PvPConsole {
 public:
  explicit PvPConsole(const std::string& FEN)
      : m_board(fen::parse(FEN)), m_state(GameState::Default()) {}

  void focus() {
    m_running = true;
    while (m_running) {
      loop();
    }
  }

 private:
  void loop() {
    printState();
    processInput();
  }

  void printState() {
    print_display(m_board);
    m_moveCollector.reset();
    generate_moves(m_board, m_state, m_moveCollector);
    int n = 1;
    for (const runtime_move& move : m_moveCollector.moves()) {
      if (n % 10 == 1) std::cout << "[";
      std::cout << toAlgebraicNotation(m_board, m_state, move);
      if (n % 10 == 0)
        std::cout << "]" << std::endl;
      else
        std::cout << ", ";
      n++;
    }
    std::cout << std::endl;
  }

  void processInput() {
    std::string in;
    std::cin >> in;
    // trim input.
    in.erase(in.find_last_not_of(' ') + 1);
    in.erase(0, in.find_first_not_of(' '));
    try {
      runtime_move move = parseAlgebraicNotation(m_board, m_state, in);
      //apply move.
      m_board = m_board.applyMove(m_state, move);
      m_state = m_state.applyMove(move);
    } catch (std::invalid_argument& err) {
      std::cout << "Not a valid move! try again" << std::endl;
    }
  }

 private:
  Board m_board;
  GameState m_state;
  RuntimeMoveCollector m_moveCollector;
  bool m_running = false;
};
