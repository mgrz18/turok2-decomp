/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004058B0.c, func_004058B0, with Turok 2's layouts. */
#include "common.h"
#include "globals/frame.h"
#include "globals/render.h"

/* Frees and reallocates the frame buffers, the depth buffer and the
 * display-list memory. A handle points at a struct whose first word is the
 * block address. */

typedef struct {
    s32 addr; /* 0x0 */
} MemHandle;

typedef struct {
    s32 ready;          /* 0x00 */
    MemHandle *gfx;     /* 0x04 */
    MemHandle *dynamic; /* 0x08 */
    MemHandle *fb[3];   /* 0x0C */
    MemHandle *depth;   /* 0x18 */
    MemHandle *vtx;     /* 0x1C */
} GfxPool;

extern MemHandle *CCache__Allocate(s32 heap, s32 size, s32 flags, char *name); /* allocate */
extern MemHandle *CCache__AllocateAt(s32 heap, s32 addr, s32 size, char *name);  /* claim a fixed block */
extern void CCache__Deallocate(s32 heap, MemHandle *handle);                     /* free */
extern u32 GetMemSize(void);                                             /* osGetMemSize */
extern void memset(void *dst, s32 value, u32 size);                  /* memset */

extern char D_800C1810[];
extern char D_800C181C[];
extern char D_800C182C[];
extern char D_800C184C[];
extern char D_800C185C[];
extern char D_800C1864[];
extern s32 gCFB[]; /* frame buffers */

void GfxPool_Alloc_2(GfxPool *pool) {
    u32 mb = 0x100000;
    u32 i;
    s32 j;
    s32 n;
    MemHandle *depth;
    MemHandle *fb;
    MemHandle *block;
    u32 size;
    u32 base;

    if (pool->gfx != 0) {
        CCache__Deallocate(0, pool->gfx);
    }
    if (pool->dynamic != 0) {
        CCache__Deallocate(0, pool->dynamic);
    }
    if (pool->depth != 0) {
        CCache__Deallocate(0, pool->depth);
    }
    if (pool->vtx != 0) {
        CCache__Deallocate(0, pool->vtx);
    }
    for (n = 0; n < 3; n++) {
        if (pool->fb[n] != 0) {
            CCache__Deallocate(0, pool->fb[n]);
        }
    }

    pool->ready = 0;
    pool->gfx = 0;
    pool->dynamic = 0;
    pool->depth = 0;
    pool->vtx = 0;
    for (j = 2; j >= 0; j--) {
        pool->fb[j] = 0;
    }

    if (GetMemSize() >= mb * 6) {
        size = 0x54600;
        base = GetMemSize() | 0x80000000;
        depth = CCache__AllocateAt(0, base - mb, size, D_800C1810);
        fb = CCache__AllocateAt(0, base - mb * 2, 0xFD200, D_800C181C);
        memset((void *) fb->addr, 0, 0xFD200);
        memset((void *) depth->addr, 0, 0x54600);
        pool->depth = depth;
        pool->fb[0] = fb;
        gDepthBuffer = pool->depth->addr;
        for (i = 0; i < gTotalFramebuffers; i++) {
            gCFB[i] = fb->addr + i * size;
        }
    } else {
        size = 0x1ECC0;
        base = 0x80400000;
        block = CCache__AllocateAt(0, base - (gTotalFramebuffers + 1) * size, (gTotalFramebuffers + 1) * size, D_800C182C);
        pool->fb[0] = block;
        for (i = 0; i < gTotalFramebuffers; i++) {
            gCFB[i] = block->addr + i * size;
        }
        gDepthBuffer = block->addr + size * i;
    }
    for (i = 0; i < gTotalFramebuffers; i++) {
        memset((void *) gCFB[i], 0, size);
    }
    memset((void *) gDepthBuffer, 0, size);
    pool->gfx = CCache__Allocate(0, (gDisplayListSize + gLineListSize) * 16, 0x23, D_800C184C);
    pool->dynamic = CCache__Allocate(0, 0x8000, 0x23, D_800C185C);
    pool->vtx = CCache__Allocate(0, 0x22000, 0x23, D_800C1864);
    gDisplayList1 = pool->gfx->addr;
    gLineList0 = gDisplayList1 + gDisplayListSize * 8;
    gDisplayList0 = gLineList0 + gLineListSize * 8;
    gLineList1 = gDisplayList0 + gDisplayListSize * 8;
    gRspFifoSize = 0x8000;
    gAudioHeapSize = 0x22000;
    gRspFifoBuffer = (u64 *) pool->dynamic->addr;
    gAudioHeapBase = pool->vtx->addr;
    for (i = 0; i < gTotalFramebuffers; i++)
        ;
    pool->ready = 1;
}
