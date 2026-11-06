/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_3.c, func_802515F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void CList__RemoveHead(CList *arg0);
extern void CCache__Compact(s32, s32);
extern CList D_800D81F4;
extern void CList__Construct(CList *list, s32 prevOffset, s32 nextOffset);
extern s32 CList__AddTail(CList *list, void *node);
extern s32 CHeap__Alloc(void *, s32);
extern void memmove(u32, u32, s32);
extern void CList__InsertBefore(void *, s32, s32);
void CCache__Compact(s32 unused, s32 arg1) {
    extern void CList__Remove(void *, s32);
    extern void CHeap__Free(void *, s32);
    extern s32 D_800D1CC0[3];
    extern s32 D_800D81D8[];
    CList queue;
    Node80254C10 *node;
    Node80254C10 *next;
    Node80254C10 *scan;
    u32 split;
    s32 *arena_end;
    s32 *arena;
    s32 *pool;
    CList__Construct(&queue, 0x20, 0x24);
    if (arg1 != 0 || D_800D81D8[0] == 0) {
        D_800D81D8[0] = D_800D81D8[7];
        if (D_800D81D8[7] == 0) {
            goto drain;
        }
    }
    pool = D_800D81D8;
    arena_end = &D_800D1CC0[2];
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
    } while (D_800D81D8[0] != 0);
drain:
    node = queue.m_pHead;
    while (node != 0) {
        next = node->next;
        CList__RemoveHead(&queue);
        scan = D_800D81F4.m_pHead;
        while (scan != 0) {
            if (scan->start > node->start) {
                CList__InsertBefore(&D_800D81F4, (s32)scan, (s32)node);
                break;
            }
            scan = scan->next;
        }
        if (scan == 0) {
            CList__AddTail(&D_800D81F4, node);
        }
        node->flags |= 0x1000;
        node = next;
    }
}
