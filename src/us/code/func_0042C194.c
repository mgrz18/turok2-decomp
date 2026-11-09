/* WRONG CUT: REAL CScene__FindWarpDests size 0x228 (splat cut it into CScene__FindWarpDests + func_0042C2DC + func_0042C338 + func_0042C38C). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_29.CPP, func_8044D220_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game.h"
#include "protos/list.h"
#include "protos/iset.h"

typedef struct ListRec {   /* a CList (structs/list.h) whose m_nCount is read as count */
#ifdef TARGET_PORT
    /* 0x00 */ void *pad0[2];
    /* 0x08 */ s32 pad8[2];
#else
    /* 0x00 */ u8 pad0[0x10];
#endif
    /* 0x10 */ s32 count;
    /* 0x14 */ u8 pad14[4];
} ListRec;

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

extern char D_800A5D8C[];

extern void CCache__LoadPersistantIndex(s32, s32 *, sptr, char *);
extern void **CCache__LoadPersistantSubBlock(s32, sptr, s32, s32, void *, s32, char *);
extern s32 BinaryRange(s32 *keys, s32 count, s32 key, s32 *first, s32 *last);
extern void CCache__Deallocate(s32, void *);

/* Copies up to max placed objects of the given kind (-1: any) whose key matches into out; returns how many. */
s32 CScene__FindWarpDests(void *world, s32 kind, s32 key, Placed *out, s32 max) {
    ListRec list;
    Node nodes[64];
    s32 resource;
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
    CCache__LoadPersistantIndex(found, &resource, pool, D_800A5D8C);
    handle = CCache__LoadPersistantSubBlock(found, resource, 8, pool, world, 0, "warp dests");
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
