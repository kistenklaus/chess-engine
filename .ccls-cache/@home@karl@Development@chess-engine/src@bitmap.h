#pragma once
#include <cinttypes>
#include <iostream>
#include <sstream>
#include <string>

#define bitmap_t uint64_t

std::string bitmap_to_bitboard_string(bitmap_t bitmap);
