#ifndef STRUCTS_RECT_H
#define STRUCTS_RECT_H

/* A rectangle on the ground plane (X and Z): the bounds of a world sector
 * (CWorldSector.m_pBounds), of a floor triangle (func_0026B130,
 * func_0026CF34) or of a collision sweep (func_0023A968).
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

typedef struct CRect {
    /* 0x00 */ f32 minX;
    /* 0x04 */ f32 minZ;
    /* 0x08 */ f32 maxX;
    /* 0x0C */ f32 maxZ;
} CRect; /* size 0x10 */

#endif
