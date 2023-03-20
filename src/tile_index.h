#pragma once


//for debugging this should be set to something like int16 because otherwise
// overflows and underflow's might not be detected
//for best performance this should be uint8_t
#include <iostream>
#include <cinttypes>


typedef uint8_t tile_index_t;

struct tile_index {
  explicit tile_index(tile_index_t value) : m_value(value){}
  tile_index_t m_value;
};

std::ostream& operator<<(std::ostream& cout, const tile_index& index);
