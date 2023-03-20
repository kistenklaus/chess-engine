#pragma once

#include <iostream>
#include <string>

#include "Board.h"
#include "display.h"
#include "fen.h"
#include "move.h"
#include "move_generation.h"

class PvPConsoleMoveReceiver {
 public:
  template <figure_type figure>
  void move(bitmap_t origin, bitmap_t target, move_flag flag) {
    m_moves.emplace_back(origin, target, figure, flag);
  }
  [[nodiscard]] const std::vector<runtime_move>& moves() const {
    return m_moves;
  }
  void reset(){
    m_moves.clear();
  }
 private:
  std::vector<runtime_move> m_moves;
};

class PvPConsole {
 public:
  explicit PvPConsole(const std::string& FEN)
      : m_board(fen::parse(FEN)), m_state(BoardState::Default()) {}

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
    m_receiver.reset();
    generate_moves(m_board, m_state, m_receiver);
    int n = 1;
    for(const runtime_move& move : m_receiver.moves()){
      if(n % 4 == 1)std::cout << "[";
      std::cout << move;
      if(n % 4 == 0)std::cout << "]" << std::endl;
      else std::cout << ", ";
      n++;
    }
    std::cout << std::endl;
  }

  void processInput() {
    std::string in;
    std::cin >> in;
  }

 private:
  Board m_board;
  BoardState m_state;
  PvPConsoleMoveReceiver m_receiver;
  bool m_running = false;
};
