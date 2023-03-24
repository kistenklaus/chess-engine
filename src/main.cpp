
#include <chrono>
#include <iostream>

#include "PerfTRecurser.h"
#include "PvPConsole.h"

#include "MagicLookup.h"
int main() {
  // PvPConsole pvpConsole(fen::DEFAULT);
  // pvpConsole.focus();

  auto perfT =
      PerfTRecursion<GameState::Default()>(fen::parse(fen::DEFAULT), 5);

  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  auto t1 = high_resolution_clock::now();
  perfT.start();
  auto t2 = high_resolution_clock::now();
  std::cout << perfT.result() << std::endl;
  auto ms_int = duration_cast<milliseconds>(t2 - t1);
  std::cout << "took : " <<ms_int << std::endl;
}
