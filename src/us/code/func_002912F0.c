#include "common.h"

/* A bump allocator over a buffer. */
typedef struct {
    /* 0x0 */ s32 base;             /* 16-byte aligned */
    /* 0x4 */ s32 cur;
    /* 0x8 */ s32 size;
    /* 0xC */ s32 count;
} Arena;

void alHeapInit(Arena *arg0, s32 arg1, s32 arg2) {
    s32 temp_v1;

    temp_v1 = 0x10 - (arg1 & 0xF);
    if (temp_v1 != 0x10) {
        arg0->base = (s32) (arg1 + temp_v1);
    } else {
        arg0->base = arg1;
    }
    arg0->size = arg2;
    arg0->count = 0;
    arg0->cur = (s32) arg0->base;
}
