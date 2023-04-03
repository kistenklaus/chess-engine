#pragma once
#include <cinttypes>
#include <iostream>
#include <sstream>
#include <string>

typedef uint64_t bitmap_t;

namespace bitmap {
std::string toString(bitmap_t map);
} // namespace bitmap
