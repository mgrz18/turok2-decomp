#include "common.h"

#ifdef TARGET_PORT
/* the host layout of ALHeap (structs/libaudio.h): pointers, then len */
typedef struct {
    /* 0x00 */ uptr base;
    /* 0x04 */ uptr cur;
    /* 0x08 */ s32 size;
} Heap;
typedef uptr HeapWord;
#else
typedef struct {
    /* 0x00 */ s32 base;
    /* 0x04 */ u32 cur;
    /* 0x08 */ s32 size;
} Heap;
typedef u32 HeapWord;
#endif

/* Takes a 16-byte aligned count*size block from the heap, or 0 if it does not fit. */
HeapWord alHeapDBAlloc(s32 arg0, s32 arg1, Heap *arg2, s32 arg3, s32 arg4) {
    HeapWord temp_a1;
    HeapWord temp_v1;
    HeapWord var_a3;

    temp_a1 = arg2->cur;
    temp_v1 = temp_a1 + (((arg3 * arg4) + 0xF) & ~0xF);
    var_a3 = 0;
    if ((HeapWord) (arg2->base + arg2->size) >= temp_v1) {
        var_a3 = temp_a1;
        arg2->cur = temp_v1;
    }
    return var_a3;
}
