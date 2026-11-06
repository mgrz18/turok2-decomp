/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002512C8.c, func_80251328_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/cache.h"

extern s32 CCache__FreeOldestMemEntry(s32 unused0, s32 unused1, s32 keep_recent);
extern CMemEntry *D_800D81E4;
extern CMemEntry *D_800D81D8;
extern CMemEntry **D_800D81D4;
extern s32 D_800D8DAC;
extern s32 D_800D8DF0;
extern char D_800D1CC0;
extern char D_800D81E0;
extern char D_800D81F4;
extern void CCache__DetachCacheEntry(void *, CMemEntry *);
extern s32 CHeap__Free(void *, s32);
extern void CList__Remove(void *, CMemEntry *);
static __inline__ CMemEntry *find_stale(CMemEntry **recent) {
    CMemEntry *node;
    for (node = D_800D81E4; node != 0; node = node->last) {
        if (!(node->flags & 0x702)) {
            if ((u32)(D_800D8DF0 - node->age) >= 5) {
                return node;
            }
            *recent = node;
            break;
        }
    }
    for (; node != 0; node = node->last) {
        if (!(node->flags & 0x702) && (u32)(D_800D8DF0 - node->age) >= 5) {
            return node;
        }
    }
    return 0;
}
s32 CCache__FreeOldestMemEntry(s32 unused0, s32 unused1, s32 keep_recent) {
    CMemEntry *recent;
    CMemEntry *node;
    s32 result;
    recent = 0;
    node = find_stale(&recent);
    if (node == 0) {
        if (keep_recent == 0) {
            node = recent;
        }
    }
    if (node != 0) {
        CCache__DetachCacheEntry(0, node);
        result = CHeap__Free(&D_800D1CC0, node->data);
        CList__Remove(&D_800D81E0, node);
        if (node->flags & 0x1000) {
            CList__Remove(&D_800D81F4, node);
        }
        if (D_800D81D8 == node) {
            D_800D81D8 = 0;
            node->flags = 0;
        } else {
            node->flags = 0;
        }
        *(CMemEntry **)(D_800D8DAC * 4 + (char *)D_800D81D4) = node;
        D_800D8DAC++;
        return result;
    }
    return 0;
}
