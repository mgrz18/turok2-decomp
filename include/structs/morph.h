#ifndef STRUCTS_MORPH_H
#define STRUCTS_MORPH_H

/* The morphing trail vertices (Morph_BlendVertices, Morph_BlendVerticesTex, from the
 * trail draw func_00234348): a vertex buffer blended between two point sets, the
 * colours between two colour sets. MorphOwner / MorphClock are the trail's Swoosh /
 * SwooshNode (structs/swoosh.h) seen through the fields the blend reads. Hoisted in
 * round 21 from the three files that each had a copy. */

#include "common.h"
#include "structs/vector.h"

typedef struct MorphPoint {         /* 0x14 bytes */
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ u8 pad0C[0x8];
} MorphPoint;

typedef struct MorphItem {          /* 0xC bytes */
    /* 0x00 */ u8 colorA[4];
    /* 0x04 */ u8 colorB[4];
    /* 0x08 */ u16 pointA;
    /* 0x0A */ u16 pointB;
} MorphItem;

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

typedef struct MorphTiming {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ f32 fadeTime;
    /* 0x08 */ f32 duration;
} MorphTiming;

#ifdef TARGET_PORT
/* Round 20 i3: the N64 pad put `remaining` at host offset 0x30 (the owner pointer), not at
 * Swoosh.time (0x3C on the host): the trails faded by the bits of a pointer. */
#include "structs/swoosh.h"
typedef struct MorphOwner {     /* structs/swoosh.h Swoosh */
    void *prev;
    void *next;
    MorphTiming *timing;        /* info */
    void *tex0;
    void *tex1;
    s32 texture[2];
    void *owner;
    s32 id;
    f32 remaining;              /* time */
} MorphOwner;
_Static_assert(__builtin_offsetof(MorphOwner, timing) == __builtin_offsetof(Swoosh, info), "MorphOwner.timing");
_Static_assert(__builtin_offsetof(MorphOwner, remaining) == __builtin_offsetof(Swoosh, time), "MorphOwner.remaining");
#else
typedef struct MorphOwner {     /* structs/swoosh.h Swoosh */
    /* 0x00 */ void *prev;
    /* 0x04 */ void *next;
    /* 0x08 */ MorphTiming *timing;
    /* 0x0C */ u8 padC[0x24 - 0xC];
    /* 0x24 */ f32 remaining;
} MorphOwner;
#endif

typedef struct MorphClock {     /* structs/swoosh.h SwooshNode */
    /* 0x00 */ void *prev;
    /* 0x04 */ void *next;
    /* 0x08 */ f32 time;
} MorphClock;

#endif
