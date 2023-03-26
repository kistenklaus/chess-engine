#include "cli.h"

#include <iostream>
#include <sstream>

#include "movegen_movestack.h"
#include "notation.h"
#include "perfT.h"

namespace cli {

void internal::initState() {
  g_state.m_position = ChessPosition::StartPosition();
  g_state.m_running = false;
}

void internal::processInput() {
  std::string line;
  std::getline(std::cin, line, '\n');
  std::vector<std::string> split;
  std::stringstream ss;
  std::string str;
  while (std::getline(ss, str, ' ')) {
    split.push_back(str);
  }
  parseLine(split);
}

internal::result internal::commandGo(const std::vector<std::string>& args) {
  const std::string& subcommand = args[0];
  if (subcommand == "perft") {
    int depth;
    if (args.size() > 1) {
      std::stringstream ss;
      ss << args[1];
      ss >> depth;
    } else {
      depth = 6;
    }
    perfT::run(g_state.m_position, depth);
  } else {
    return result{.m_failed = true, .m_message = "fuck"};
  }
  return result{.m_failed = false};
}

internal::result internal::commandMove(const std::vector<std::string>& args) {
  if (args.empty()) {
    return {.m_failed = true, .m_message = "move requires 1 argument at least"};
  }
  for (const std::string& moveNotation : args) {
    runtime_move move = notation::parse(g_state.m_position, moveNotation);
    // apply move.
    const Board nextBoard =
        g_state.m_position.board().applyMove(g_state.m_position.state(), move);
    const GameState nextState = runtimeStateTransition(
        g_state.m_position.state(), move.m_figure, move.m_flag);
    bitmap_t epTarget = 0;  // TODO determine epTarget.
    g_state.m_position.update(nextBoard, nextState, epTarget);
  }
  return {.m_failed = false};
}
internal::result internal::commandPrintBoard() {
  return result{.m_failed = false};
}

void run() {
  using namespace internal;
  initState();
  g_state.m_running = true;
  while (g_state.m_running) {
    processInput();
  }
}
void parseLine(std::vector<std::string>& input) {
  using namespace internal;
  if (input.empty()) return;
  std::string command = input[0];
  result result;
  if (command == "d") {
    result = commandPrintBoard();
  } else if (command == "go") {
    result =
        commandGo(std::vector<std::string>(input.begin() + 1, input.end()));
  } else if (command == "move") {
    result =
        commandMove(std::vector<std::string>(input.begin() + 1, input.end()));
  } else if (command == "exit") {
    g_state.m_running = false;
    result = {.m_failed = false};
  } else {
    result = {.m_failed = true, .m_message = "invalid command"};
  }
  if (result.m_failed) {
    std::cout << result.m_message << std::endl;
  }
}
}  // namespace cli