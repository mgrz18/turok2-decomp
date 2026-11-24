/* WRONG CUT: REAL CScene__FindWarpDests size 0x228 (splat cut it into CScene__FindWarpDests + func_0042C2DC + func_0042C338 + func_0042C38C). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_29.CPP, func_8044D220_de, with Turok 2's layouts. */
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
#include "protos/list.h"
#include "protos/iset.h"
#include "structs/twin_sceneload.h"


typedef struct Node {
    /* 0x00 */ s32 index;
    /* 0x04 */ void *prev;
    /* 0x08 */ void *next;
} Node;

typedef struct Placed {
    /* 0x00 */ s32 data[4];
    /* 0x10 */ s16 kind;
    /* 0x12 */ s16 unk12;
} Placed; /* 0x14 */


s32 BinaryRange(void *array, s32 count, u32 key, s32 *first, s32 *last);

/* Copies up to max placed objects of the given kind (-1: any) whose key matches into out; returns how many. */
s32 CScene__FindWarpDests(void *world, s32 kind, s32 key, Placed *out, s32 max) {
    ListRec list;
    Node nodes[64];
    sptr resource;          /* the index (a pointer) */
    s32 first;
    s32 last;
    s32 found;
    s32 *keys;
    s32 *objectTable;
    Placed *objects;
    Node *node;
    s32 i;
    s32 match;
    s32 count;
    void **handle;
    void *table;
    s32 pool;

    found = 0;
    pool = gEngineApp.m_CartridgeIndex;
    CCache__LoadPersistantIndex(found, &resource, pool, sCSceneCacheDesc_CartridgeIndex);
    handle = CCache__LoadPersistantSubBlock_v(found, resource, 8, pool, world, 0, "warp dests");
    table = *handle;
    count = (keys = CISet__GetBlock(table, 0))[1];
    keys += 2;
    objectTable = CISet__GetBlock(table, 1);
    objects = (Placed *)(objectTable + 2);
    if (BinaryRange(keys, count, key, &first, &last) != 0) {
        CList__Construct(&list, T2_OFFSETOF(Node, prev), T2_OFFSETOF(Node, next));
        node = nodes;
        for (i = first; i <= last; i++) {
            match = 1;
            if (kind != -1) {
                match = objects[i].kind == kind;
            }
            if (match) {
                node->index = i;
                CList__AddHead(&list, node);
                node++;
            }
        }
        for (max--; max != -1; max--) {
            if (list.count == 0) {
                break;
            }
            node = CList__GetRandom(&list);
            out[found++] = objects[node->index];
            CList__Remove(&list, node);
        }
    }
    CCache__Deallocate(0, handle);
    return found;
}
