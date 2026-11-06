#include "common.h"
#include "globals/engine.h"

/* Per-view vertex buffers: 0x12C0 bytes each, the current one at 0x2584. */
typedef struct {
    /* 0x0000 */ u8 buffers[0x2580];
    /* 0x2580 */ s32 count;
    /* 0x2584 */ void *current;
} VtxBuffers;


void RibbonVtxPool_Reset(VtxBuffers *arg0) {
    s32 i = gEvenOdd;
    arg0->count = 0x12C;
    arg0->current = (s8 *)arg0 + i * 0x12C0;
}
