/* WRONG CUT: REAL func_0042C194 size 0x228 (splat cut it into func_0042C194 + func_0042C2DC + func_0042C338 + func_0042C38C). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_29.CPP, func_8044D220_de, with Turok 2's layouts. */
#include "common.h"

typedef struct ListRec {
    /* 0x00 */ u8 pad0[0x10];
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

extern s32 D_800F7074;
extern char D_800A5D8C[];
extern char D_800A5FC4[];

extern void func_00205B94(s32, s32 *, s32, char *);
extern void **func_00205D0C(s32, s32, s32, s32, void *, s32, char *);
extern void *func_002017D4(void *table, s32 index);
extern s32 func_00266D8C(s32 *keys, s32 count, s32 key, s32 *first, s32 *last);
extern void CList__Construct(ListRec *list, s32 prevOffset, s32 nextOffset);
extern s32 CList__AddHead(ListRec *list, Node *node);
extern Node *func_002009F0(ListRec *list);
extern void CList__Remove(ListRec *list, Node *node);
extern void func_002052D8(s32, void *);

/* Copies up to max placed objects of the given kind (-1: any) whose key matches into out; returns how many. */
s32 func_0042C194(void *world, s32 kind, s32 key, Placed *out, s32 max) {
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
    pool = D_800F7074;
    func_00205B94(found, &resource, pool, D_800A5D8C);
    handle = func_00205D0C(found, resource, 8, pool, world, 0, D_800A5FC4);
    table = *handle;
    count = (keys = func_002017D4(table, 0))[1];
    keys += 2;
    objectTable = func_002017D4(table, 1);
    objects = (Placed *)(objectTable + 2);
    if (func_00266D8C(keys, count, key, &first, &last) != 0) {
        CList__Construct(&list, 4, 8);
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
            node = func_002009F0(&list);
            out[found++] = objects[node->index];
            CList__Remove(&list, node);
        }
    }
    func_002052D8(0, handle);
    return found;
}
