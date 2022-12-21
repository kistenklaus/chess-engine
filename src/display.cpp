#include "display.h"

void print_display(const Board& board){
  using namespace std;
  
  const std::string line_seperator = "+---+---+---+---+---+---+---+---+";
  static const std::string outside_color = "40";
  static const std::string background_color = "107";
  static const std::string board_color = "100";
  static const std::string white_color = "37";
  static const std::string black_color = "30";
  system("clear");

  cout << "\033[" << background_color
       << "m                                                                   "
          "         "
       << "\033[0m" << std::endl;
  cout << "\033[" << background_color
       << "m                                                                   "
          "         "
       << "\033[0m" << std::endl;

  for (int i = 7; i >= 0; i--) {
    cout << "\033[" << background_color << "m     "
         << "\033[0m";
    cout << "\033[" << board_color << "m" << line_seperator << "\033[0m";
    cout << "\033[" << background_color
         << "m                                      "
         << "\033[0m";
    cout << std::endl;

    cout << "\033[" << background_color << "m "
         << "\033[0m";
    cout << "\033[" << outside_color << "m " << i + 1 << " \033[0m";
    cout << "\033[" << background_color << "m "
         << "\033[0m";
    cout << "\033[" << board_color << "m|\033[0m";
    for (int j = 0; j < 8; j++) {
      tile_index_t index = i * 8 + j;
      const figure_t figure = board.get_figure_at(index);
      if (figure.type()) {
        if (figure.color() == WHITE) {
          cout << "\033[" << board_color << "m\x1B[" << white_color << "m "
               << figure << " \033[0m";
        } else {
          cout << "\033[" << board_color << "m\x1B[" << black_color << "m "
               << figure << " \033[0m";
        }

      } else {
        cout << "\033[" << board_color << "m   \033[0m";
      }
      cout << "\033[" << board_color << "m|\033[0m";
    }

    cout << "\033[" << background_color
         << "m                                      "
         << "\033[0m";

    cout << std::endl;
  }
  cout << "\033[" << background_color << "m     "
       << "\033[0m";
  cout << "\033[" << board_color << "m" << line_seperator << "\033[0m";

  cout << "\033[" << background_color
       << "m                                      "
       << "\033[0m" << std::endl;

  cout << "\033[" << background_color
       << "m                                                                   "
          "         "
       << "\033[0m" << std::endl;

  cout << "\33[" << background_color << "m     "
       << "\033[0m";
  for (int i = 0; i < 8; i++) {
    cout << "\033[" << background_color << "m "
         << "\033[0m";
    cout << "\033[" << outside_color << "m " << (char)('a' + i) << " "
         << "\033[0m";
    // cout << "\033[" << background_color << "m " << "\033[0m";
  }

  cout << "\033[" << background_color
       << "m                              "
          "         "
       << "\033[0m" << std::endl;

  cout << "\033[" << background_color
       << "m                                                                   "
          "         "
       << "\033[0m";
  cout << std::endl;
}
