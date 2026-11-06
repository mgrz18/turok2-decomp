#ifndef STRUCTS_RUMBLE_H
#define STRUCTS_RUMBLE_H

/* The rumble effects (Rumble_Init to RumbleCurve_GetValue): a RumblePool
 * holds RumbleEffect nodes on a free and an active CList (links at 0x00 and
 * 0x04, RumblePool_Init). An effect plays a clip at a position: two curves
 * (the amplitude and the lifetime, RumbleCurve_Advance) scaled by the distance
 * to the listener (Rumble_GetLevelAt). gRumblePool is the world's pool; each
 * controller has a pool of 3 of its own (ControllerProfile.rumblePool).
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"
#include "structs/list.h"
#include "structs/vector.h"

typedef struct RumbleCurve {
    /* 0x00 */ void *curve;         /* CScene__GetCurve */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} RumbleCurve; /* size 0xC */

typedef struct RumbleEffect {
    /* 0x00 */ struct RumbleEffect *prev;
    /* 0x04 */ struct RumbleEffect *next;
    /* 0x08 */ void *clip;
    /* 0x0C */ CVector3 position;
    /* 0x18 */ f32 radius;
    /* 0x1C */ f32 scale;
    /* 0x20 */ RumbleCurve amplitude;
    /* 0x2C */ RumbleCurve lifetime;
    /* 0x38 */ struct RumbleEffect **owner; /* cleared when the effect ends */
} RumbleEffect; /* size 0x3C */

typedef struct RumblePool {
    /* 0x00 */ CList free;
    /* 0x14 */ CList active;
    /* 0x28 */ f32 baseLevel;       /* the level the effects add to; 0 after RumblePool_Init/FreeAll */
} RumblePool; /* size 0x2C */

#endif
