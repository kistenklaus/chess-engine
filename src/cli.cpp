#include "cli.h"

#include <sys/ioctl.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "movegen_constant_collection_decl.h"
#include "notation.h"
#include "perfT.h"

namespace cli {

struct result {
private:
  bool m_failed;
  std::string m_message;
  result(bool failed, std::string message)
      : m_failed(failed), m_message(std::move(message)) {}

public:
  result() = default;
  [[nodiscard]] bool isError() const { return m_failed; }
  [[nodiscard]] const std::string &message() const { return m_message; }
  static result succ() { return {false, ""}; }
  static result error(const std::string &message) { return {true, message}; }
};

struct state {
public:
  static state &get() {
    static state instance;
    return instance;
  }

private:
  explicit state() = default;

public:
  state(state const &) = delete;
  void operator=(const state &) = delete;

  [[nodiscard]] const ChessPosition &position() const { return m_position; }
  void setPosition(const ChessPosition &position) { m_position = position; }
  void setPosition(const ChessPosition &&position) { m_position = position; }
  [[nodiscard]] bool isRunning() const { return m_running; }
  void stop() { m_running = false; }
  void start() { m_running = true; }

private:
  ChessPosition m_position;
  bool m_running = false;
};

static result parseLine(std::vector<std::string> &input);

void initState() {
  state::get().setPosition(ChessPosition::StartPosition());
  state::get().start();
}

static void processInput() {
  std::string line;
  std::getline(std::cin, line);
  std::vector<std::string> split;
  std::stringstream ss(line);
  std::string str;
  while (std::getline(ss, str, ' ')) {
    split.push_back(str);
  }
  result result = parseLine(split);
  if (result.isError()) {
    std::cout << result.message() << std::endl;
  }
}

static result commandMove(const std::vector<std::string> &args) {
  if (args.empty()) {
    return result::error("move requires 1 argument at least");
  }
  for (const std::string &moveNotation : args) {
    runtime_move move{};
    try {
      move = notation::parse(state::get().position(), moveNotation);
    } catch (const std::exception &e) {
      std::string msg = std::string(e.what());
      return result::error(msg);
    }
    // apply move.
    const Board nextBoard = state::get().position().board().applyMove(
        state::get().position().state(), move);
    const GameState nextState = runtimeStateTransition(
        state::get().position().state(), move.m_figure, move.m_flag);
    bitmap_t epTarget = 0;
    if (move.m_flag == MOVE_FLAG_DOUBLE_PAWN_PUSH) {
      epTarget = move.m_target;
    }
    ChessPosition newPosition = ChessPosition(nextBoard, nextState, epTarget);
    state::get().setPosition(newPosition);
  }
  return result::succ();
}

static result commandPosition(const std::vector<std::string> &args) {
  if (args.empty()) {
    return result::error("position required at least one argument");
  }
  ChessPosition position;
  unsigned int index = 0;
  if (args[0] == "startpos") {
    position = ChessPosition::StartPosition();
    index += 1;
  } else if(args[0] == "kiwi" || args[0] == "kiwipete"){
    position = fen::parsePosition("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    index += 1;
  } else if(args[0] == "cp3"){
    position = fen::parsePosition("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    index += 1;
  } else if(args[0] == "cp4"){
    position = fen::parsePosition("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

    index += 1;
  }else if(args.size() >= 6){
    std::stringstream ss;
    for(unsigned int i=0;i<6;i++){
      ss << args[i];
      if(i != 5)ss << ' ';
    }
    position = fen::parsePosition(ss.str());
    index += 6;
  }else{
    return result::error("not a valid position");
  }
  state::get().setPosition(position);
  if (index < args.size() && args[index] == "move") {
    return commandMove(std::vector<std::string>(args.begin() + index + 1, args.end()));
  }
  return result::succ();
}

static result commandGo(const std::vector<std::string> &args) {
  const std::string &subcommand = args[0];
  if (subcommand == "perft") {
    int depth;
    if (args.size() > 1) {
      std::stringstream ss;
      ss << args[1];
      ss >> depth;
    } else {
      depth = 6;
    }
    try {
      perfT::run(state::get().position(), depth);
    } catch (const std::exception &e) {
      return result::error("failed while executing perft");
    }
  } else {
    return result::error("perft required 1 argument");
  }
  return result::succ();
}

static result subCommandDebugMovegen() {
  const std::vector<runtime_move> moves =
      movegen::constant_collection::runtime_entry_call(state::get().position());
  for (const runtime_move &move : moves) {
    const std::string notation =
        notation::toString(state::get().position(), move);
    // TODO pad notation to 7 chars.
    std::cout << notation;
    std::cout << " [";
    std::cout << "origin = ";
    std::cout << notation::toString(move.m_origin);
    std::cout << ",";
    std::cout << "figure = ";
    switch (move.m_figure) {
    case PAWN:
      std::cout << "PAWN";
      break;
    case BISHOP:
      std::cout << "BISHOP";
      break;
    case KNIGHT:
      std::cout << "KNIGHT";
      break;
    case ROOK:
      std::cout << "ROOK";
      break;
    case QUEEN:
      std::cout << "QUEEN";
      break;
    case KING:
      std::cout << "KING";
      break;
    }
    std::cout << ",";
    std::cout << "flag = ";
    switch (move.m_flag) {
    case MOVE_FLAG_SILENT:
      std::cout << "SILENT";
      break;
    case MOVE_FLAG_SHORT_CASTLE:
      std::cout << "SHORT_CASTLE";
      break;
    case MOVE_FLAG_LONG_CASTLE:
      std::cout << "LONG_CASTLE";
      break;
    case MOVE_FLAG_DOUBLE_PAWN_PUSH:
      std::cout << "DOUBLE_PAWN_PUSH";
      break;
    case MOVE_FLAG_LEFT_ROOK:
      std::cout << "LEFT_ROOK_MOVED";
      break;
    case MOVE_FLAG_RIGHT_ROOK:
      std::cout << "RIGHT_ROOK_MOVED";
      break;
    case MOVE_FLAG_CAPTURE:
      std::cout << "CAPTURE";
      break;
    case MOVE_FLAG_EN_PASSANT:
      std::cout << "EN_PASSANT";
      break;
    case MOVE_FLAG_PROMOTE:
      std::cout << "PROMOTE";
      break;
    }
    std::cout << "]";
    std::cout << std::endl;
  }
  return result::succ();
}

static result commandDebug(const std::vector<std::string> &args) {
  if (args.empty()) {
    return result::error("debug command requires one argument at least");
  }
  const std::string &subcommand = args[0];
  if (subcommand == "movegen") {
    return subCommandDebugMovegen();
  }

  return result::error("not a valid debug subcommand");
}

static void printBoardMoveHelper(const std::vector<runtime_move> &moves,
                                 int &current, int maxLength) {
  unsigned int c = 0;
  while (c < maxLength && current < moves.size()) {
    std::string notation =
        notation::toString(state::get().position(), moves[current]);
    if (c + notation.size() + 2 < maxLength) {
      std::cout << notation;
      current += 1;
      if (current != moves.size()) {
        std::cout << ", ";
      }
      c += notation.size() + 2;
    } else {
      break;
    }
  }
}

static result commandPrintBoard() {
  int cols = 80;
  struct winsize ws {};
  if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != 0 &&
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0 &&
      ioctl(STDERR_FILENO, TIOCGWINSZ, &ws) != 0) {
    std::cout << "failed to get window width" << std::endl;
  } else {
    cols = ws.ws_col;
  }
  if (cols < 60) {
    cols = 60;
  }

  std::vector<runtime_move> moves =
      movegen::constant_collection::runtime_entry_call(state::get().position());
  int moveIndex = 0;
  std::cout << cols << std::endl;
  int maxMoveLength = cols - 40;

  std::cout << " +---+---+---+---+---+---+---+---+     ";
  if (state::get().position().state().turn() == WHITE) {
    std::cout << "\033[47m";
  } else {
    std::cout << "\033[40m";
  }
  std::cout << " ";
  std::cout << "\033[0m";
  std::cout << " ";
  printBoardMoveHelper(moves, moveIndex, maxMoveLength);
  std::cout << std::endl;
  figure figure;
  color_t color;
  for (int rank = 7; rank >= 0; rank--) {
    std::cout << " |";
    for (int file = 0; file < 8; file++) {
      bool occupied = state::get().position().getSquareOccupation(
          rank * 8 + file, &figure, &color);
      bool marked = false;
      marked |= state::get().position().state().whiteHasShortCastle() &&
                rank == 0 && file == 7;
      marked |= state::get().position().state().whiteHasLongCastle() &&
                rank == 0 && file == 0;
      marked |= state::get().position().state().blackHasShortCastle() &&
                rank == 7 && file == 7;
      marked |= state::get().position().state().blackHasLongCastle() &&
                rank == 7 && file == 0;
      if (state::get().position().state().hasEnPassant()) {
        bitmap_t epTarget = state::get().position().epTarget();
        unsigned int epRank = epTarget / 8;
        unsigned int epFile = epTarget % 8;
        marked |= epRank == rank && epFile == file;
      }

      if (marked) {
        std::cout << "\033[97m";
      }

      if (!occupied) {
        std::cout << "   ";
      } else {
        int charOffset = (color == WHITE) ? ('A' - 'a') : 0;
        std::cout << " ";
        if (figure == PAWN) {
          std::cout << (char)('p' + charOffset);
        } else if (figure == BISHOP) {
          std::cout << (char)('b' + charOffset);
        } else if (figure == KNIGHT) {
          std::cout << (char)('n' + charOffset);
        } else if (figure == ROOK) {
          std::cout << (char)('r' + charOffset);
        } else if (figure == QUEEN) {
          std::cout << (char)('q' + charOffset);
        } else if (figure == KING) {
          std::cout << (char)('k' + charOffset);
        } else {
          throw std::runtime_error("wtf");
        }
        std::cout << " ";
      }
      if (marked) {
        std::cout << "\033[0m";
      }
      std::cout << "|";
    }
    std::cout << " " << (rank + 1) << "   ";
    if (state::get().position().state().turn() == WHITE) {
      std::cout << "\033[47m";
    } else {
      std::cout << "\033[40m";
    }
    std::cout << " ";
    std::cout << "\033[0m";

    std::cout << " ";
    printBoardMoveHelper(moves, moveIndex, maxMoveLength);
    std::cout << std::endl;
    std::cout << " +---+---+---+---+---+---+---+---+     ";
    if (state::get().position().state().turn() == WHITE) {
      std::cout << "\033[47m";
    } else {
      std::cout << "\033[40m";
    }
    std::cout << " ";
    std::cout << "\033[0m";
    std::cout << " ";
    printBoardMoveHelper(moves, moveIndex, maxMoveLength);
    std::cout << std::endl;
  }
  std::cout << "   a   b   c   d   e   f   g   h       ";
  if (state::get().position().state().turn() == WHITE) {
    std::cout << "\033[47m";
  } else {
    std::cout << "\033[40m";
  }
  std::cout << " ";
  std::cout << "\033[0m";
  std::cout << std::endl;
  return result::succ();
}

static result commandPrintState() {
  const GameState &state = state::get().position().state();
  std::cout << "=========Game-State=========" << std::endl;
  std::cout << "-turn               : "
            << ((state.turn() == WHITE) ? "white" : "black") << std::endl;
  std::cout << "-en-passant         : "
            << (state.hasEnPassant() ? "true" : "false") << std::endl;
  std::cout << "-en-passant-target  : " << state::get().position().epTarget()
            << std::endl;
  std::cout << "-white-short-castle : "
            << (state.whiteHasShortCastle() ? "true" : "false") << std::endl;
  std::cout << "-white-long-castle  : "
            << (state.whiteHasLongCastle() ? "true" : "false") << std::endl;
  std::cout << "-black-short-castle :"
            << (state.blackHasShortCastle() ? "true" : "false") << std::endl;
  std::cout << "-black-long-castle  :"
            << (state.blackHasLongCastle() ? "true" : "false") << std::endl;
  return result::succ();
};

static result commandTime(std::vector<std::string> args) {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  auto t1 = high_resolution_clock::now();
  result result = parseLine(args);

  auto t2 = high_resolution_clock::now();
  auto ms_int = duration_cast<milliseconds>(t2 - t1);
  std::cout << "took " << ms_int << std::endl;
  return result;
}

void run() {
  std::cout << "chadfish" << std::endl;
  initState();
  while (state::get().isRunning()) {
    processInput();
  }
}

static result parseLine(std::vector<std::string> &input) {
  if (input.empty())
    return result::succ();
  std::string command = input[0];
  result result;
  if (command == "d" || command == "display") {
    result = commandPrintBoard();
  } else if (command == "s") {
    result = commandPrintState();
  } else if (command == "position") {
    result = commandPosition(
        std::vector<std::string>(input.begin() + 1, input.end()));
  } else if (command == "go") {
    result =
        commandGo(std::vector<std::string>(input.begin() + 1, input.end()));
  } else if (command == "move") {
    result =
        commandMove(std::vector<std::string>(input.begin() + 1, input.end()));
  } else if (command == "debug") {
    result =
        commandDebug(std::vector<std::string>(input.begin() + 1, input.end()));
  } else if (command == "time") {
    result =
        commandTime(std::vector<std::string>(input.begin() + 1, input.end()));
  } else if (command == "exit") {
    state::get().stop();
    result = result::succ();
  } else {
    result = result::error("invalid command");
  }
  return result;
}
} // namespace cli
