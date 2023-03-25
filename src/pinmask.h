#pragma once
#include "bitmap.h"

struct pinmask_t {
  pinmask_t() : hv(0), d12(0){}
  pinmask_t(bitmap_t hv, bitmap_t d12) : hv(hv), d12(d12){}
  bitmap_t hv;
  bitmap_t d12;
};
