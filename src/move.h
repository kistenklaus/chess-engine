#pragma once
#include <cinttypes>

#include "Board.h"
#include "bitmap.h"
#include "x86utils.h"

typedef uint32_t move_flag;

const move_flag MOVE_FLAG_NONE = 0;
const move_flag MOVE_FLAG_RCASTLE = 1;
const move_flag MOVE_FLAG_LCASTLE = 2;
const move_flag MOVE_FLAG_EN_PASSANT = 4;
const move_flag MOVE_FLAG_DOUBLE_PAWN_PUSH = 8;
const move_flag MOVE_FLAG_CAPTURE = 16;
const move_flag MOVE_FLAG_CHECK = 32;
