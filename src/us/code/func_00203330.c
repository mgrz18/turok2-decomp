/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_3.c, func_802515F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void CList__RemoveHead(CList *arg0);
extern void CCache__Compact(s32, s32);
extern CList gCache_memLockedList;
extern void CList__Construct(CList *list, s32 prevOffset, s32 nextOffset);
extern s32 CList__AddTail(CList *list, void *node);
extern s32 CHeap__Alloc(void *, s32);
extern void memmove(u32, u32, s32);
extern void CList__InsertBefore(void *, s32, s32);
void CCache__Compact(s32 unused, s32 arg1) {
    extern void CList__Remove(void *, s32);
    extern void CHeap__Free(void *, s32);
    extern s32 gHeap[3];
    extern s32 gCache_currentCompressPos[];
    CList queue;
    Node80254C10 *node;
    Node80254C10 *next;
    Node80254C10 *scan;
    u32 split;
    s32 *arena_end;
    s32 *arena;
    s32 *pool;
    CList__Construct(&queue, 0x20, 0x24);
    if (arg1 != 0 || gCache_currentCompressPos[0] == 0) {
        gCache_currentCompressPos[0] = gCache_currentCompressPos[7];
        if (gCache_currentCompressPos[7] == 0) {
            goto drain;
        }
    }
    pool = gCache_currentCompressPos;
    arena_end = &gHeap[2];
    arena = arena_end - 2;
    do {
        node = (Node80254C10 *)pool[0];
        pool[0] = *(s32 *)&node->next;
        if (!(*(s32 *)&node->flags & 0x701) && (u32)(*arena_end + 0x20) < node->start) {
            CHeap__Free(arena, node->start);
            split = CHeap__Alloc(arena, node->end);
            if (split < node->start) {
                memmove(split, node->start, node->end);
                node->start = split;
                CList__Remove(&pool[7], (s32)node);
                CList__AddTail(&queue, node);
            }
            if (arg1 == 0) {
                break;
            }
        }
    } while (gCache_currentCompressPos[0] != 0);
drain:
    node = queue.m_pHead;
    while (node != 0) {
        next = node->next;
        CList__RemoveHead(&queue);
        scan = gCache_memLockedList.m_pHead;
        while (scan != 0) {
            if (scan->start > node->start) {
                CList__InsertBefore(&gCache_memLockedList, (s32)scan, (s32)node);
                break;
            }
            scan = scan->next;
        }
        if (scan == 0) {
            CList__AddTail(&gCache_memLockedList, node);
        }
        node->flags |= 0x1000;
        node = next;
    }
}
