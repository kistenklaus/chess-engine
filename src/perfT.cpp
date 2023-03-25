//
// Created by karl on 25/03/23.
//
#include "perfT.h"

#include <chrono>

#include "move_generation.h"

namespace perfT {
namespace internal {

template <GameState state, int depth>
class RecursiveMoveCounter {
 public:
  explicit RecursiveMoveCounter(const Board board) : m_board(board) {}

  template <figure_type figure, move_flag flag>
  void move(bitmap_t origin, bitmap_t target) {
    if constexpr (depth == 0) {
      m_count++;
    } else {
      compiletime_move<figure, flag> move = {.m_origin = origin,
                                             .m_target = target};

      Board nextBoard = m_board.applyMove<state>(move);
      constexpr GameState nextState =
          compiletimeStateTransition<state, figure, flag>();

      Movegen::PrepareEnumeration<state, figure, flag, depth>(m_board, move);
      auto recursion = RecursiveMoveCounter<nextState, depth - 1>(nextBoard);
      m_count += recursion.run();
    }
  }

  unsigned long run() {
    Movegen::Enumerate<state, depth, RecursiveMoveCounter<state, depth>>(
        m_board, *this);
    return m_count;
  }

  [[nodiscard]] unsigned long result() const { return m_count; }

 private:
  const Board m_board;
  unsigned long m_count = 0;
};

template <GameState state, int depth>
class RecursiveProfiler {
 public:
  explicit RecursiveProfiler(const Board& board) : m_board(board) {}

  template <figure_type figure, move_flag flag>
  void move(bitmap_t origin, bitmap_t target) {
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    if constexpr (depth == 0) {
      m_count++;
      std::cout << 1 << std::endl;
    } else {
      compiletime_move<figure, flag> move = {.m_origin = origin,
                                             .m_target = target};

      Board nextBoard = m_board.applyMove<state>(move);
      constexpr GameState nextState =
          compiletimeStateTransition<state, figure, flag>();

      Movegen::PrepareEnumeration<state, figure, flag, depth>(m_board, move);
      auto recursion = RecursiveMoveCounter<nextState, depth - 1>(nextBoard);

      auto t1 = high_resolution_clock::now();
      unsigned long count = recursion.run();
      auto t2 = high_resolution_clock::now();
      auto ms_int = duration_cast<milliseconds>(t2 - t1);
      m_count += count;
      std::cout << count << "   took : " << ms_int << std::endl;
    }
  }

  unsigned long run() {
    Movegen::internal::initMovegen(0);
    Movegen::internal::Movestack::init<state, depth>(m_board);
    Movegen::Enumerate<state, depth, RecursiveProfiler<state, depth>>(m_board,
                                                                      *this);
    return m_count;
  }

 private:
  const Board m_board;
  unsigned long m_count = 0;
};

template <GameState state>
unsigned long perfT(const Board& board, int depth) {
  switch (depth) {
    case 0:
      return RecursiveProfiler<state, 0>(board).run();
    case 1:
      return RecursiveProfiler<state, 1>(board).run();
    case 2:
      return RecursiveProfiler<state, 2>(board).run();
    case 3:
      return RecursiveProfiler<state, 3>(board).run();
    case 4:
      return RecursiveProfiler<state, 4>(board).run();
    case 5:
      return RecursiveProfiler<state, 5>(board).run();
    case 6:
      return RecursiveProfiler<state, 6>(board).run();
    default:
      throw std::runtime_error(
          "[Unimplemented] depth compiletime invocation is not implemented");
  }
}

unsigned long perfT(const Board& board, const GameState& state,
                                     int depth) {
  if (state.turn()) {
    if (state.hasEnPassant()) {
      if (state.whiteHasLongCastle()) {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, true, true, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, true, true, true, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, true, true, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, true, true, true, false, false)>(
                  board, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, true, false, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, true, true, false, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, true, false, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, true, true, false, false, false)>(
                  board, depth);
            }
          }
        }
      } else {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, false, true, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, true, false, true, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, false, true, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, true, false, true, false, false)>(
                  board, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, false, false, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, true, false, false, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, false, false, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, true, false, false, false, false)>(
                  board, depth);
            }
          }
        }
      }
    } else {
      if (state.whiteHasLongCastle()) {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, true, true, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, false, true, true, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, true, true, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, false, true, true, false, false)>(
                  board, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, true, false, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, false, true, false, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, true, false, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, false, true, false, false, false)>(
                  board, depth);
            }
          }
        }
      } else {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, false, true, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, false, false, true, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, false, true, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, false, false, true, false, false)>(
                  board, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, false, false, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, false, false, false, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, false, false, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(true, false, false, false, false, false)>(
                  board, depth);
            }
          }
        }
      }
    }
  } else {
    if (state.hasEnPassant()) {
      if (state.whiteHasLongCastle()) {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, true, true, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, true, true, true, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, true, true, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, true, true, true, false, false)>(
                  board, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, true, false, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, true, true, false, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, true, false, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, true, true, false, false, false)>(
                  board, depth);
            }
          }
        }
      } else {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, false, true, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, true, false, true, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, false, true, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, true, false, true, false, false)>(
                  board, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, false, false, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, true, false, false, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, false, false, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, true, false, false, false, false)>(
                  board, depth);
            }
          }
        }
      }
    } else {
      if (state.whiteHasLongCastle()) {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, true, true, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, false, true, true, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, true, true, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, false, true, true, false, false)>(
                  board, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, true, false, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, false, true, false, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, true, false, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, false, true, false, false, false)>(
                  board, depth);
            }
          }
        }
      } else {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, false, true, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, false, false, true, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, false, true, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, false, false, true, false, false)>(
                  board, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, false, false, true, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, false, false, false, true, false)>(
                  board, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, false, false, false, true)>(
                  board, depth);
            } else {
              return perfT<GameState(false, false, false, false, false, false)>(
                  board, depth);
            }
          }
        }
      }
    }
  }
}
}  // namespace internal

unsigned long run(const std::string& fen, int depth) {
  const Board& board = fen::parse(fen);
  const GameState state = GameState::Default();
  return internal::perfT(board, state, depth);
}

}  // namespace perfT
