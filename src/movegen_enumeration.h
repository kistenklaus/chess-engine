/**
 * @author      : karl (karlsasssie@gmail.com)
 * @created     : 03/04/2023
 * @filename    : movegen_enumeration
 */
#pragma once
#include "movegen_constant_enumeration.h"


template<typename MoveReceiver>
void (*constant_recursive_calls[64 * 10])(const Board&, MoveReceiver& receiver);

template<typename MoveReceiver>
void (*constant_entry_call[64 * 10])(const Board&, MoveReceiver& receiver);

template<int depth, typename MoveReceiver>
void entry_call(const GameState& state, MoveReceiver& receiver){
}
