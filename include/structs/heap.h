#ifndef STRUCTS_HEAP_H
#define STRUCTS_HEAP_H

/* The main heap (CHeap__Construct to CHeap__GetFreeStats; Turok 3's heap.h,
 * references/turok3/include/heap.h). Every block, free or in use, starts
 * with a CHeapBlock; Turok 2's header is 0x20 bytes (Turok 3's 0x18), so the
 * data a block hands out starts at block + 0x20. The free blocks are linked
 * through freeLast/freeNext from gHeap.freeHead, every block in address
 * order through usedLast/usedNext from gHeap.usedHead.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

typedef struct CHeapBlock {
    /* 0x00 */ struct CHeapBlock *freeLast;
    /* 0x04 */ struct CHeapBlock *freeNext;
    /* 0x08 */ struct CHeapBlock *usedLast;
    /* 0x0C */ struct CHeapBlock *usedNext;
    /* 0x10 */ u32 used;            /* the bytes in use, header included (0x20 when free) */
    /* 0x14 */ u32 free;            /* the free bytes after them */
    /* 0x18 */ u8 pad18[8];         /* keeps the data 16-aligned */
} CHeapBlock; /* size 0x20 */

typedef struct CHeap {
    /* 0x00 */ u8 *start;
    /* 0x04 */ u8 *end;
    /* 0x08 */ CHeapBlock *freeHead;
    /* 0x0C */ CHeapBlock *freeTail;
    /* 0x10 */ CHeapBlock *usedHead;
} CHeap; /* size 0x14 */

#endif
