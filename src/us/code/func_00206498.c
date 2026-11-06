#include "common.h"
#include "structs/cache.h"
#include "globals/cache.h"

void CList__AddTail(CList *list, void *element);
void CList__InsertBefore(CList *list, void *before, void *element);

/* Files a CMemEntry in the locked list, which is sorted by address. */
void CCache__AddToAddressList(s32 unused, CMemEntry *entry) {
    CMemEntry *scan;

    scan = gCache_memLockedList.m_pHead;
    if (scan != NULL) {
loop_1:
        if (scan->data > entry->data) {
            CList__InsertBefore(&gCache_memLockedList, scan, entry);
        } else {
            scan = scan->lockedNext;
            if (scan != NULL) {
                goto loop_1;
            }
        }
        if (scan == NULL) {
            goto block_5;
        }
    } else {
block_5:
        CList__AddTail(&gCache_memLockedList, entry);
    }
    entry->flags = entry->flags | MEMENTRY_FLAG_ON_LOCKED;
}
