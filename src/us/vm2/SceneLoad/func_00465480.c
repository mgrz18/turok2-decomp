/* WRONG CUT: REAL CScene__FindNearestWarpDest_2 size 0x1A0 (splat cut it into CScene__FindNearestWarpDest_2 + func_0042C620 + func_0042C6BC). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_29.CPP, func_8044D3E8_de, with Turok 2's layouts. */
#include "common.h"
#include "protos/cache.h"
/* The request CCache__LoadPersistantSubBlock leaves in v0 (protos/cache.h declares the N64 definition
 * void): the N64 symbol by another C name, returning it; the port's definition returns it. */
#ifdef TARGET_PORT
#define CCache__LoadPersistantSubBlock_v(...) ((void **) CCache__LoadPersistantSubBlock(__VA_ARGS__))
#else
void **CCache__LoadPersistantSubBlock_v() __asm__("CCache__LoadPersistantSubBlock");
#endif
#include "globals/strings.h"
#include "structs/game.h"
#include "protos/iset.h"
#include "structs/twin_sceneload.h"





extern f32 CVector3__Distance(void *a, void *b);

/* Finds the point of the given type nearest to position; copies it to out and returns its value (0 when none). */
s32 CScene__FindNearestWarpDest_2(void *world, void *position, s32 type, SearchPoint *out) {
    s32 i;
    s32 best;
    SearchValues *values;
    SearchPoints *points;
    s32 *vals;
    SearchPoint *pts;
    s32 count;
    f32 distance;
    f32 minimum;
    sptr resource;          /* the index (a pointer) */
    s32 pool;
    void **handle;
    void *table;

    pool = gEngineApp.m_CartridgeIndex;
    CCache__LoadPersistantIndex(0, &resource, pool, sCSceneCacheDesc_CartridgeIndex);
    i = 0;
    best = 0;
    handle = CCache__LoadPersistantSubBlock_v(0, resource, 8, pool, world, 0, sCSceneCacheDesc_WarpDests);
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
