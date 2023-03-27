//
// Created by karl on 27/03/23.
//
#include "movegen_movestack.h"


const int MAX_DEPTH = 32;
bitmap_t g_KingAttack[MAX_DEPTH];
bitmap_t g_EnemyKingAttack[MAX_DEPTH];
checkmask_t g_Checkmask[MAX_DEPTH];

bitmap_t g_EnPassantTarget = {};
bitmap_t g_RookPin = {};
bitmap_t g_BishopPin = {};
