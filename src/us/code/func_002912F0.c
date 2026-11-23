#include "common.h"

/* A bump allocator over a buffer. */
/* the host layout of ALHeap (structs/libaudio.h) */
typedef struct {
    /* 0x0 */ sptr base;            /* 16-byte aligned */
    /* 0x4 */ sptr cur;
    /* 0x8 */ s32 size;
    /* 0xC */ s32 count;
} Arena;

#define ARENA_ADDR(x) ((sptr)(x)) /* the (s32) casts truncated a heap above 4 GB (macOS, --high-memory) */

void alHeapInit(Arena *arg0, sptr arg1, s32 arg2) {
    s32 temp_v1;

    temp_v1 = 0x10 - (arg1 & 0xF);
    if (temp_v1 != 0x10) {
        arg0->base = ARENA_ADDR(arg1 + temp_v1);
    } else {
        arg0->base = arg1;
    }
    arg0->size = arg2;
    arg0->count = 0;
    arg0->cur = ARENA_ADDR(arg0->base);
}
