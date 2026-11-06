/* WRONG CUT: REAL CScene__FindNearestWarpDest size 0x1A0 (splat cut it into CScene__FindNearestWarpDest + func_0042C620 + func_0042C6BC). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_29.CPP, func_8044D3E8_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game.h"

typedef struct SearchPoint {
    /* 0x00 */ f32 position[3];
    /* 0x0C */ u32 fieldC;
    /* 0x10 */ s16 type;
    /* 0x12 */ s16 field12;
} SearchPoint; /* 0x14 */

typedef struct SearchValues {
    s32 field0;
    s32 count;
    s32 values[1];
} SearchValues;

typedef struct SearchPoints {
    s32 field0;
    s32 field4;
    SearchPoint points[1];
} SearchPoints;

extern char D_800A5D8C[];
extern char D_800A5FC4[];

extern void CCache__LoadPersistantIndex(s32, s32 *, s32, char *);
extern void **CCache__LoadPersistantSubBlock(s32, s32, s32, s32, void *, s32, char *);
extern void *CISet__GetBlock(void *table, s32 index);
extern f32 CVector3__Distance(void *a, void *b);
extern void CCache__Deallocate(s32, void *);

/* Finds the point of the given type nearest to position; copies it to out and returns its value (0 when none). */
s32 CScene__FindNearestWarpDest(void *world, void *position, s32 type, SearchPoint *out) {
    s32 i;
    s32 best;
    SearchValues *values;
    SearchPoints *points;
    s32 *vals;
    SearchPoint *pts;
    s32 count;
    f32 distance;
    f32 minimum;
    s32 resource;
    s32 pool;
    void **handle;
    void *table;

    pool = gEngineApp.m_CartridgeIndex;
    CCache__LoadPersistantIndex(0, &resource, pool, D_800A5D8C);
    i = 0;
    best = 0;
    handle = CCache__LoadPersistantSubBlock(0, resource, 8, pool, world, 0, D_800A5FC4);
    table = *handle;
    values = CISet__GetBlock(table, 0);
    count = values->count;
    vals = values->values;
    points = CISet__GetBlock(table, 1);
    minimum = 3.4028235e38f;
    pts = points->points;
    for (; i < count; i++) {
        if (pts[i].type == type) {
            distance = CVector3__Distance(&pts[i], position);
            if (distance < minimum) {
                minimum = distance;
                best = i;
            }
        }
    }
    if (best != 0) {
        *out = pts[best];
    }
    CCache__Deallocate(0, handle);
    if (best != 0) {
        return vals[best];
    }
    return 0;
}
