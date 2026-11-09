/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_3.c, func_802515F0_de.
 * The same code in Turok 2. */

#include "common.h"
#include "structs/cache.h"
#include "globals/cache.h"
#include "globals/sysrc.h"
#include "protos/list.h"
#include "protos/cache.h"

extern u8 *CHeap__Alloc(CHeap *heap, s32 size);
extern void CHeap__Free(CHeap *heap, u8 *data);
extern void memmove(void *dst, void *src, s32 size);

/* Moves unlocked blocks down the heap (free, then allocate again lower) and
 * files the moved ones back in the address-sorted locked list; one block per
 * call, or all of them. */
void CCache__Compact(s32 unused, s32 all) {
    CList moved;
    CMemEntry *entry;
    CMemEntry *next;
    CMemEntry *scan;
    u8 *newData;

    CList__Construct(&moved, 0x20, 0x24);
    if (all != 0 || gCache.currentCompressPos == NULL) {
        gCache.currentCompressPos = gCache.memLockedList.m_pHead;
        if (gCache.memLockedList.m_pHead == NULL) {
            goto drain;
        }
    }
    do {
        entry = gCache.currentCompressPos;
        gCache.currentCompressPos = entry->lockedNext;
        if (!(entry->flags & 0x701) && (uptr)gHeap.freeHead + sizeof(CHeapBlock) < (uptr)entry->data) {
            CHeap__Free(&gHeap, entry->data);
            newData = CHeap__Alloc(&gHeap, entry->size);
            if (newData < (u8 *)entry->data) {
                memmove(newData, entry->data, entry->size);
                entry->data = newData;
                CList__Remove(&gCache.memLockedList, entry);
                CList__AddTail(&moved, entry);
            }
            if (all == 0) {
                break;
            }
        }
    } while (gCache.currentCompressPos != NULL);
drain:
    entry = moved.m_pHead;
    while (entry != NULL) {
        next = entry->lockedNext;
        CList__RemoveHead(&moved);
        scan = gCache_memLockedList.m_pHead;
        while (scan != NULL) {
            if ((uptr)scan->data > (uptr)entry->data) {
                CList__InsertBefore(&gCache_memLockedList, scan, entry);
                break;
            }
            scan = scan->lockedNext;
        }
        if (scan == NULL) {
            CList__AddTail(&gCache_memLockedList, entry);
        }
        entry->flags |= MEMENTRY_FLAG_ON_LOCKED;
        entry = next;
    }
}
