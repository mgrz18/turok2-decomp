#include "common.h"

/* Per-view vertex buffers: 0x12C0 bytes each, the current one at 0x2584. */
typedef struct {
    /* 0x0000 */ u8 buffers[0x2580];
    /* 0x2580 */ s32 count;
    /* 0x2584 */ void *current;
} VtxBuffers;

extern s32 D_800B6D1C;          /* current view */

void func_00235FC4(VtxBuffers *arg0) {
    s32 i = D_800B6D1C;
    arg0->count = 0x12C;
    arg0->current = (s8 *)arg0 + i * 0x12C0;
}
