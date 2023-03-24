//
// Created by karl on 24/03/23.
//
#pragma once
#include "x86utils.h"
#include "bitmap.h"

#include "Movemap.h"

namespace MagicLookup{

compiletime bitmap_t Knight(bitmap_t tile){
  return Chess_Lookup::Lookup_Pext::Knight(tile);
}

compiletime bitmap_t King(bitmap_t tile){
  return Chess_Lookup::Lookup_Pext::King(tile);
}

compiletime bitmap_t Bishop(bitmap_t tile, bitmap_t occupy){
  return Chess_Lookup::Lookup_Pext::Bishop(tile, occupy);
}

compiletime bitmap_t Rook(bitmap_t tile, bitmap_t occupy){
  return Chess_Lookup::Lookup_Pext::Rook(tile, occupy);
}

compiletime bitmap_t Queen(bitmap_t tile, bitmap_t occupy){
  return Chess_Lookup::Lookup_Pext::Queen(tile, occupy);
}

}

