/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004058B0.c, func_004058B0, with Turok 2's layouts. */
#include "common.h"

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

extern MemHandle *func_00204EDC(s32 heap, s32 size, s32 flags, char *name); /* allocate */
extern MemHandle *func_00204FD0(s32 heap, s32 addr, s32 size, char *name);  /* claim a fixed block */
extern void func_002052D8(s32 heap, MemHandle *handle);                     /* free */
extern u32 func_00266B80(void);                                             /* osGetMemSize */
extern void func_00266C5C(void *dst, s32 value, u32 size);                  /* memset */

extern char D_800C1810[];
extern char D_800C181C[];
extern char D_800C182C[];
extern char D_800C184C[];
extern char D_800C185C[];
extern char D_800C1864[];
extern u32 D_800C2210; /* frame buffer count */
extern s32 D_800C2214;
extern s32 D_800C2218;
extern s32 D_80130A88[]; /* frame buffers */
extern s32 D_80130A78;   /* depth buffer */
extern s32 D_80130A40;
extern s32 D_80130A44;
extern s32 D_80130A48;
extern s32 D_80130A4C;
extern s32 D_80130A50;
extern s32 D_80130A7C;
extern s32 D_80130A80;
extern s32 D_80130A84;

void func_00425220(GfxPool *pool) {
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
        func_002052D8(0, pool->gfx);
    }
    if (pool->dynamic != 0) {
        func_002052D8(0, pool->dynamic);
    }
    if (pool->depth != 0) {
        func_002052D8(0, pool->depth);
    }
    if (pool->vtx != 0) {
        func_002052D8(0, pool->vtx);
    }
    for (n = 0; n < 3; n++) {
        if (pool->fb[n] != 0) {
            func_002052D8(0, pool->fb[n]);
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

    if (func_00266B80() >= mb * 6) {
        size = 0x54600;
        base = func_00266B80() | 0x80000000;
        depth = func_00204FD0(0, base - mb, size, D_800C1810);
        fb = func_00204FD0(0, base - mb * 2, 0xFD200, D_800C181C);
        func_00266C5C((void *) fb->addr, 0, 0xFD200);
        func_00266C5C((void *) depth->addr, 0, 0x54600);
        pool->depth = depth;
        pool->fb[0] = fb;
        D_80130A78 = pool->depth->addr;
        for (i = 0; i < D_800C2210; i++) {
            D_80130A88[i] = fb->addr + i * size;
        }
    } else {
        size = 0x1ECC0;
        base = 0x80400000;
        block = func_00204FD0(0, base - (D_800C2210 + 1) * size, (D_800C2210 + 1) * size, D_800C182C);
        pool->fb[0] = block;
        for (i = 0; i < D_800C2210; i++) {
            D_80130A88[i] = block->addr + i * size;
        }
        D_80130A78 = block->addr + size * i;
    }
    for (i = 0; i < D_800C2210; i++) {
        func_00266C5C((void *) D_80130A88[i], 0, size);
    }
    func_00266C5C((void *) D_80130A78, 0, size);
    pool->gfx = func_00204EDC(0, (D_800C2214 + D_800C2218) * 16, 0x23, D_800C184C);
    pool->dynamic = func_00204EDC(0, 0x8000, 0x23, D_800C185C);
    pool->vtx = func_00204EDC(0, 0x22000, 0x23, D_800C1864);
    D_80130A80 = pool->gfx->addr;
    D_80130A4C = D_80130A80 + D_800C2214 * 8;
    D_80130A84 = D_80130A4C + D_800C2218 * 8;
    D_80130A50 = D_80130A84 + D_800C2214 * 8;
    D_80130A44 = 0x8000;
    D_80130A7C = 0x22000;
    D_80130A48 = pool->dynamic->addr;
    D_80130A40 = pool->vtx->addr;
    for (i = 0; i < D_800C2210; i++)
        ;
    pool->ready = 1;
}
