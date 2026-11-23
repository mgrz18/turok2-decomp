/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004058B0.c, func_004058B0, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"
#include "globals/frame.h"
#include "globals/render.h"
#include "globals/framebuffer.h"
#include "structs/frame.h"
#include "structs/cache.h"

/* Frees and reallocates the frame buffers, the depth buffer and the
 * display-list memory. A handle points at a struct whose first word is the
 * block address. */



extern CMemEntry *CCache__Allocate(s32 heap, s32 size, s32 flags, char *name); /* allocate */
extern CMemEntry *CCache__AllocateAt(s32 heap, sptr addr, s32 size, char *name);  /* claim a fixed block */
extern void CCache__Deallocate(sptr heap, CMemEntry *handle);                     /* free */
extern u32 GetMemSize(void);                                             /* osGetMemSize */
extern void memset(void *dst, s32 value, uptr size);                  /* memset */

extern char sGfxPoolText_AudioHeap[]; /* "audio heap": the byte after it is the linker's fill (0x0B), not a zero the C could give */

void GfxPool_Alloc_2(CGfxPool *pool) {
    u32 mb = 0x100000;
    u32 i;
    s32 j;
    s32 n;
    CMemEntry *depth;
    CMemEntry *fb;
    CMemEntry *block;
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
        depth = CCache__AllocateAt(0, base - mb, size, "Z-Buffer");
        fb = CCache__AllocateAt(0, base - mb * 2, 0xFD200, "Display Buffer");
        memset((void *) (uptr)fb->data, 0, 0xFD200);
        memset((void *) (uptr)depth->data, 0, 0x54600);
        pool->depth = depth;
        pool->fb[0] = fb;
        gDepthBuffer = (uptr)pool->depth->data;
        for (i = 0; i < gTotalFramebuffers; i++) {
            gCFB[i] = (uptr)fb->data + i * size;
        }
    } else {
        size = 0x1ECC0;
        base = 0x80400000;
        block = CCache__AllocateAt(0, base - (gTotalFramebuffers + 1) * size, (gTotalFramebuffers + 1) * size, "color frame buffers + z buffer");
        pool->fb[0] = block;
        for (i = 0; i < gTotalFramebuffers; i++) {
            gCFB[i] = (uptr)block->data + i * size;
        }
        gDepthBuffer = (uptr)block->data + size * i;
    }
    for (i = 0; i < gTotalFramebuffers; i++) {
        memset((void *) gCFB[i], 0, size);
    }
    memset((void *) gDepthBuffer, 0, size);
    pool->gfx = CCache__Allocate(0, (gDisplayListSize + gLineListSize) * (2 * GFX_SIZE), 0x23, "display lists");
    pool->dynamic = CCache__Allocate(0, 0x8000, 0x23, "fifo");
    pool->vtx = CCache__Allocate(0, 0x22000, 0x23, sGfxPoolText_AudioHeap);
    gDisplayList1 = (uptr)pool->gfx->data;
    gLineList0 = gDisplayList1 + gDisplayListSize * GFX_SIZE;
    gDisplayList0 = gLineList0 + gLineListSize * GFX_SIZE;
    gLineList1 = gDisplayList0 + gDisplayListSize * GFX_SIZE;
    gRspFifoSize = 0x8000;
    gAudioHeapSize = 0x22000;
    gRspFifoBuffer = (u64 *) (uptr)pool->dynamic->data;
    gAudioHeapBase = (uptr)pool->vtx->data;
    for (i = 0; i < gTotalFramebuffers; i++)
        ;
    pool->ready = 1;
}
