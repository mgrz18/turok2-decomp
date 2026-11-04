#include "common.h"

typedef struct {
    /* 0x00 */ s32 base;
    /* 0x04 */ u32 cur;
    /* 0x08 */ s32 size;
} Heap;

/* Takes a 16-byte aligned count*size block from the heap, or 0 if it does not fit. */
u32 func_002912A0(s32 arg0, s32 arg1, Heap *arg2, s32 arg3, s32 arg4) {
    u32 temp_a1;
    u32 temp_v1;
    u32 var_a3;

    temp_a1 = arg2->cur;
    temp_v1 = temp_a1 + (((arg3 * arg4) + 0xF) & ~0xF);
    var_a3 = 0;
    if ((u32) (arg2->base + arg2->size) >= temp_v1) {
        var_a3 = temp_a1;
        arg2->cur = temp_v1;
    }
    return var_a3;
}
