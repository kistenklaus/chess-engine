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

compiletime bitmap_t RookMask(bitmap_t tile){
  return Chess_Lookup::RookMask[tile];
}

compiletime bitmap_t PinBetween(bitmap_t kingTile, bitmap_t
                                enemyTile){
  return Chess_Lookup::PinBetween[kingTile * 64 + enemyTile];
}

compiletime bitmap_t CheckBetween(bitmap_t kingTile,
                                  bitmap_t enemyTile){
  return Chess_Lookup::CheckBetween[kingTile * 64 + enemyTile];
}

compiletime bitmap_t RookXRay(bitmap_t kingTile, bitmap_t occupy){
  return Chess_Lookup::Lookup_Pext::Rook_Xray(kingTile, occupy);
}

compiletime bitmap_t BishopMask(bitmap_t kingTile){
  return Chess_Lookup::BishopMask[kingTile];
}

compiletime bitmap_t BishopXRay(bitmap_t kingTile,
                                bitmap_t occupy){
  return Chess_Lookup::Lookup_Pext::Bishop_Xray(kingTile, occupy);
}

}

