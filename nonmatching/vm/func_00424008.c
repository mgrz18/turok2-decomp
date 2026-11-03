/* NON-MATCHING: 32 of 185 words differ (same size, same stack layout, all halfword stores kept): scheduling and
 * register allocation only, in the hStart part. GCC 2.8's flow pass deletes a frame store that is overwritten before
 * any read that may alias it, and a read through `mode` counts as aliasing: the ROM keeps both stores of each centre
 * (before/after -y) and of each field end (before/after -3), so the two fields are worked on side by side, with the
 * hStart read between the centre pairs and the yScale read between the end pairs (statement order is otherwise
 * irrelevant: sched1 gives identical code). Left: the ROM computes oldRight/cx from the copy oldHStart ($t8, made
 * right after the load) while this build keeps using the loaded register and copies it late (CSE canonicalises to
 * hStart, whose last use, the final store, comes after oldHStart's; every source shape tried either keeps that or lets
 * combine drop the copy), and the shared 0xFFFF0000 constant lands in $s1 late instead of $t6 first. */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/FRONTEND/8040B45C_21.CPP, func_8040BDB8_de, with Turok 2's layouts. */
#include "common.h"

/* Resizes the selected VI mode around its original horizontal and field centres, keeping the original lower
 * screen limits, then applies it. */

typedef struct {
    /* 0x00 */ u32 ctrl;
    /* 0x04 */ u32 width;
    /* 0x08 */ u32 burst;
    /* 0x0C */ u32 vSync;
    /* 0x10 */ u32 hSync;
    /* 0x14 */ u32 leap;
    /* 0x18 */ u32 hStart;
    /* 0x1C */ u32 xScale;
    /* 0x20 */ u32 vCurrent;
} ViCommonRegs;

typedef struct {
    /* 0x00 */ u32 origin;
    /* 0x04 */ u32 yScale;
    /* 0x08 */ u32 vStart;
    /* 0x0C */ u32 vBurst;
    /* 0x10 */ u32 vIntr;
} ViFieldRegs;

typedef struct {
    /* 0x00 */ u8 type;
    /* 0x04 */ ViCommonRegs comRegs;
    /* 0x28 */ ViFieldRegs fldRegs[2];
} ViMode; /* OSViMode, 0x50 */

/* A vStart register: the start line in the high half, the end line in the low half. */
typedef union {
    u32 w;
    struct {
        s16 start;
        s16 end;
    } h;
} Span;

extern s16 D_801309D0; /* selected VI mode */
extern ViMode D_801309D8;
extern ViMode D_800B7BE0[]; /* osViModeTable */
extern void *func_0029E460(void *dst, const void *src, s32 count);
extern void func_0029AA80(ViMode *mode); /* osViSetMode */

void func_00424008(s32 width, s32 height, s32 x, s32 y, s32 borderX, s32 borderY) {
    ViMode *mode = &D_801309D8;
    Span v[2];
    Span old[2];
    s16 center[2];
    u32 hStart, oldHStart;
    u32 xScale, yScale;
    s32 oldRight;
    s32 cx;

    func_0029E460(mode, &D_800B7BE0[D_801309D0], 0x50);
    old[0].w = v[0].w = mode->fldRegs[0].vStart;
    old[1].w = v[1].w = mode->fldRegs[1].vStart;
    center[0] = v[0].h.start + (v[0].h.end - v[0].h.start) / 2;
    center[1] = v[1].h.start + (v[1].h.end - v[1].h.start) / 2;
    oldHStart = hStart = mode->comRegs.hStart;
    center[0] -= y;
    center[1] -= y;
    oldRight = (s16)hStart;
    cx = (oldRight - ((s32)hStart >> 16)) / 2 + ((s32)hStart >> 16);
    cx -= x;
    hStart = (cx - width / 2 + borderX / 2) << 16;
    hStart |= (u16)(cx + width / 2 - borderX * 2);
    v[0].h.start = center[0] - height / 2 + borderY / 2;
    v[0].h.end = center[0] + height / 2 - borderY * 2;
    v[1].h.start = center[1] - height / 2 + borderY / 2;
    v[1].h.end = center[1] + height / 2 - borderY * 2;
    yScale = mode->fldRegs[0].yScale;
    yScale &= 0xFFFF0000;
    yScale |= ((height << 11) / (v[0].h.end - v[0].h.start)) & 0xFFFF;
    v[0].h.end -= 3;
    v[1].h.end -= 3;
    xScale = (width << 10) / ((s16)hStart - ((s32)hStart >> 16));
    if (old[0].h.end < v[0].h.end) {
        v[0].h.end = old[0].h.end;
    }
    if (old[1].h.end < v[1].h.end) {
        v[1].h.end = old[1].h.end;
    }
    if (oldRight < (s16)hStart) {
        hStart &= 0xFFFF0000;
        hStart |= (u16)oldHStart;
    }
    mode->fldRegs[0].yScale = yScale;
    mode->fldRegs[1].yScale = yScale;
    mode->fldRegs[0].origin = 0;
    mode->fldRegs[1].origin = width * 2;
    mode->comRegs.width = width;
    mode->comRegs.xScale = xScale;
    mode->comRegs.hStart = hStart;
    mode->fldRegs[0].vStart = v[0].w;
    mode->fldRegs[1].vStart = v[1].w;
    func_0029AA80(mode);
}
