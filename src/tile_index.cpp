//
// Created by karl on 20/03/23.
//
#include "tile_index.h"

std::ostream& operator<<(std::ostream& cout, const tile_index& index) {
  unsigned int row = index.m_value / 8 + 1;
  unsigned int column = index.m_value % 8;
  cout << (char)(((char)column) + 'a') << row;
  return cout;
}
