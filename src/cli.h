#pragma once

#include <vector>

#include "Board.h"
#include "fen.h"
#include "ChessPosition.h"

namespace cli {

namespace internal {
struct state {
  ChessPosition m_position;
  bool m_running = false;
};

struct result{
  bool m_failed;
  std::string m_message;
};

state g_state;
void initState();

void processInput();

result commandPrintBoard();

result commandMove(const std::vector<std::string>& args);

result commandGo(const std::vector<std::string>& args);
}

void run();


void parseLine(std::vector<std::string>& input);


}
