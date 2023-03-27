//
// Created by karl on 25/03/23.
//
#include "perfT.h"

#include <chrono>

#include "movegen_constant_enumeration.h"
#include "notation.h"

namespace perfT {
namespace internal {

template <GameState state, int depth>
class RecursiveMoveCounter {
 public:
  explicit RecursiveMoveCounter(const Board board) : m_board(board) {}

  template <figure figure, move_flag flag>
  void move(bitmap_t origin, bitmap_t target) {
    if constexpr (depth == 0) {
      m_count++;
    } else {
      compiletime_move<figure, flag> move = {.m_origin = origin,
                                             .m_target = target};

      Board nextBoard = m_board.applyMove<state>(move);
      constexpr GameState nextState =
          compiletimeStateTransition<state, figure, flag>();

      auto recursion = RecursiveMoveCounter<nextState, depth - 1>(nextBoard);

      movegen::constant_enumeration::prepare<state, figure, flag, depth>(
          m_board, move);
      m_count += recursion.run();
    }
  }

  force_inline unsigned long run() {
    movegen::constant_enumeration::recursive_call<
        state, depth, RecursiveMoveCounter<state, depth> >(m_board, *this);

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
  explicit RecursiveProfiler(const Board& board, bitmap_t epTarget)
      : m_board(board), m_epTarget(epTarget) {}

  template <figure figure, move_flag flag>
  void move(bitmap_t origin, bitmap_t target) {
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    compiletime_move<figure, flag> move = {.m_origin = origin,
                                           .m_target = target};

    if constexpr (depth == 0) {
      m_count++;
      std::cout << notation::toString(m_board, state, m_epTarget, move.m_origin,
                                      move.m_target, figure, flag)
                << " : 1   took : 0ms" << std::endl;
    } else {
      Board nextBoard = m_board.applyMove<state>(move);
      constexpr GameState nextState =
          compiletimeStateTransition<state, figure, flag>();

      auto recursion = RecursiveMoveCounter<nextState, depth - 1>(nextBoard);

      auto t1 = high_resolution_clock::now();

      movegen::constant_enumeration::prepare<state, figure, flag, depth>(
          m_board, move);
      unsigned long count = recursion.run();

      auto t2 = high_resolution_clock::now();
      auto ms_int = duration_cast<milliseconds>(t2 - t1);

      m_count += count;
      std::cout << notation::toString(m_board, state, m_epTarget, move.m_origin,
                                      move.m_target, figure, flag)
                << " : " << count << "   took : " << ms_int
                << std::endl;
    }
  }

  unsigned long run() {
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    std::cout << "=========DEPTH[" << (depth + 1)
              << "]=============" << std::endl;
    auto t1 = high_resolution_clock::now();
    movegen::constant_enumeration::entry_call<state, depth, RecursiveProfiler>(
        m_board, m_epTarget, *this);

    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    std::cout << std::endl;
    std::cout << "Total : " << m_count << "   took : " << ms_int << std::endl;
    return m_count;
  }

 private:
  const Board m_board;
  const bitmap_t m_epTarget;
  unsigned long m_count = 0;
};

template <GameState state>
unsigned long perfT(const Board& board, bitmap_t epTarget, int depth) {
  switch (depth) {
    case 0:
      return RecursiveProfiler<state, 0>(board, epTarget).run();
    case 1:
      return RecursiveProfiler<state, 1>(board, epTarget).run();
    case 2:
      return RecursiveProfiler<state, 2>(board, epTarget).run();
    case 3:
      return RecursiveProfiler<state, 3>(board, epTarget).run();
    case 4:
      return RecursiveProfiler<state, 4>(board, epTarget).run();
    case 5:
      return RecursiveProfiler<state, 5>(board, epTarget).run();
    case 6:
      return RecursiveProfiler<state, 6>(board, epTarget).run();
    default:
      throw std::runtime_error(
          "[Unimplemented] depth compiletime invocation is not implemented");
  }
}

unsigned long perfT(const Board& board, const GameState& state,
                    bitmap_t epTarget, int depth) {
  if (state.turn()) {
    if (state.hasEnPassant()) {
      if (state.whiteHasLongCastle()) {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, true, true, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, true, true, true, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, true, true, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, true, true, true, false, false)>(
                  board, epTarget, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, true, false, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, true, true, false, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, true, false, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, true, true, false, false, false)>(
                  board, epTarget, depth);
            }
          }
        }
      } else {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, false, true, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, true, false, true, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, false, true, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, true, false, true, false, false)>(
                  board, epTarget, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, false, false, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, true, false, false, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, true, false, false, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, true, false, false, false, false)>(
                  board, epTarget, depth);
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
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, false, true, true, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, true, true, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, false, true, true, false, false)>(
                  board, epTarget, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, true, false, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, false, true, false, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, true, false, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, false, true, false, false, false)>(
                  board, epTarget, depth);
            }
          }
        }
      } else {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, false, true, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, false, false, true, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, false, true, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, false, false, true, false, false)>(
                  board, epTarget, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, false, false, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, false, false, false, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(true, false, false, false, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(true, false, false, false, false, false)>(
                  board, epTarget, depth);
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
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, true, true, true, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, true, true, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, true, true, true, false, false)>(
                  board, epTarget, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, true, false, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, true, true, false, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, true, false, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, true, true, false, false, false)>(
                  board, epTarget, depth);
            }
          }
        }
      } else {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, false, true, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, true, false, true, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, false, true, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, true, false, true, false, false)>(
                  board, epTarget, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, false, false, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, true, false, false, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, true, false, false, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, true, false, false, false, false)>(
                  board, epTarget, depth);
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
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, false, true, true, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, true, true, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, false, true, true, false, false)>(
                  board, epTarget, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, true, false, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, false, true, false, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, true, false, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, false, true, false, false, false)>(
                  board, epTarget, depth);
            }
          }
        }
      } else {
        if (state.whiteHasShortCastle()) {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, false, true, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, false, false, true, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, false, true, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, false, false, true, false, false)>(
                  board, epTarget, depth);
            }
          }
        } else {
          if (state.blackHasLongCastle()) {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, false, false, true, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, false, false, false, true, false)>(
                  board, epTarget, depth);
            }
          } else {
            if (state.blackHasShortCastle()) {
              return perfT<GameState(false, false, false, false, false, true)>(
                  board, epTarget, depth);
            } else {
              return perfT<GameState(false, false, false, false, false, false)>(
                  board, epTarget, depth);
            }
          }
        }
      }
    }
  }
}
}  // namespace internal

unsigned long run(const ChessPosition& position, int depth) {
  return internal::perfT(position.board(), position.state(),
                         position.epTarget(), depth);
}

}  // namespace perfT
