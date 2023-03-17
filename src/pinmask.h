#pragma once
#include "bitmap.h"

struct pinmask_t {
  pinmask_t() : hv(0),d(0){}
  bitmap_t hv;
  bitmap_t d;
};
