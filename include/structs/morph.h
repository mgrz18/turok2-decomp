#ifndef STRUCTS_MORPH_H
#define STRUCTS_MORPH_H

/* The morphing trail vertices (Morph_BlendVertices, Morph_BlendVerticesTex, from the
 * trail draw Trail_DrawTextured): a vertex buffer blended between two point sets, the
 * colours between two colour sets. MorphOwner / MorphClock are the trail's Swoosh /
 * SwooshNode (structs/swoosh.h) seen through the fields the blend reads. Hoisted in
 * round 21 from the three files that each had a copy. */

#include "common.h"
#include "structs/vector.h"
#include "structs/swoosh.h"    /* MorphItem */

typedef struct MorphPoint {         /* 0x14 bytes */
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ u8 pad0C[0x8];
} MorphPoint;

typedef struct Morph {
    /* 0x00 */ s32 count;
    /* 0x04 */ MorphItem *items;
    /* 0x08 */ MorphPoint *pointsA;
    /* 0x0C */ MorphPoint *pointsB;
    /* 0x10 */ f32 scaleA;
    /* 0x14 */ f32 scaleB;
    /* 0x18 */ f32 texScaleS;
    /* 0x1C */ f32 texScaleT;
    /* 0x20 */ f32 texStepT;
} Morph;

#endif
