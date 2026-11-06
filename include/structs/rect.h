#ifndef STRUCTS_RECT_H
#define STRUCTS_RECT_H

/* A rectangle on the ground plane (X and Z): the bounds of a world sector
 * (CSceneSector.m_pBounds), of a floor triangle (CGameRegion__GetTopWithSteepNeighbours,
 * CGameRegion__GetRect) or of a collision sweep (Sweep_HitRect).
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

typedef struct CRect {
    /* 0x00 */ f32 minX;
    /* 0x04 */ f32 minZ;
    /* 0x08 */ f32 maxX;
    /* 0x0C */ f32 maxZ;
} CRect; /* size 0x10 */

#endif
