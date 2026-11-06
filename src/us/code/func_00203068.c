/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002512C8.c, func_80251328_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/cache.h"
#include "globals/cache.h"
#include "globals/sysrc.h"

extern s32 CCache__FreeOldestMemEntry(s32 unused0, s32 unused1, s32 keep_recent);
extern CMemEntry *gCache_memAgeList_m_pTail;
extern void CCache__DetachCacheEntry(void *, CMemEntry *);
extern s32 CHeap__Free(CHeap *, s32);
extern void CList__Remove(void *, CMemEntry *);
static __inline__ CMemEntry *find_stale(CMemEntry **recent) {
    CMemEntry *node;
    for (node = gCache_memAgeList_m_pTail; node != 0; node = node->last) {
        if (!(node->flags & 0x702)) {
            if ((u32)(gCache_ageCount - node->age) >= 5) {
                return node;
            }
            *recent = node;
            break;
        }
    }
    for (; node != 0; node = node->last) {
        if (!(node->flags & 0x702) && (u32)(gCache_ageCount - node->age) >= 5) {
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
        result = CHeap__Free(&gHeap, node->data);
        CList__Remove(&gCache_memAgeList, node);
        if (node->flags & 0x1000) {
            CList__Remove(&gCache_memLockedList, node);
        }
        if (gCache_currentCompressPos == node) {
            gCache_currentCompressPos = 0;
            node->flags = 0;
        } else {
            node->flags = 0;
        }
        *(CMemEntry **)(gCache_freeEntryCount * 4 + (char *)gCache_memEntriesFree) = node;
        gCache_freeEntryCount++;
        return result;
    }
    return 0;
}
