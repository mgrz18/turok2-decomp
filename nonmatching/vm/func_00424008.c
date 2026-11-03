/* NON-MATCHING: 131 of 185 words differ (size 0x2D8 vs 0x2E4): the ROM keeps both vStart words and their copies on the
 * stack (sp+0x10..0x1C) and the centres at sp+0x20, edited as s16 halves; this union-array version reproduces that
 * layout but GCC deletes the first of each pair of halfword stores (center pre/post -y, v[1].end pre/post -3) that the
 * ROM keeps, and schedules the field-1 loads earlier. */
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
    struct {
        s16 y0;
        s16 y1;
    } center;
    u32 hStart, oldHStart;
    u32 xScale, yScale;
    s32 oldRight;
    s32 cx;

    func_0029E460(mode, &D_800B7BE0[D_801309D0], 0x50);
    oldHStart = hStart = mode->comRegs.hStart;
    oldRight = (s16)hStart;
    cx = (oldRight - ((s32)hStart >> 16)) / 2 + ((s32)hStart >> 16) - x;
    hStart = ((cx - width / 2 + borderX / 2) << 16) | (u16)(cx + width / 2 - borderX * 2);

    old[0].w = v[0].w = mode->fldRegs[0].vStart;
    center.y0 = v[0].h.start + (v[0].h.end - v[0].h.start) / 2;
    center.y0 -= y;
    v[0].h.start = center.y0 - height / 2 + borderY / 2;
    v[0].h.end = center.y0 + height / 2 - borderY * 2;
    yScale = (mode->fldRegs[0].yScale & 0xFFFF0000) | (((height << 11) / (v[0].h.end - v[0].h.start)) & 0xFFFF);
    v[0].h.end -= 3;

    old[1].w = v[1].w = mode->fldRegs[1].vStart;
    center.y1 = v[1].h.start + (v[1].h.end - v[1].h.start) / 2;
    center.y1 -= y;
    v[1].h.start = center.y1 - height / 2 + borderY / 2;
    v[1].h.end = center.y1 + height / 2 - borderY * 2;
    v[1].h.end -= 3;

    xScale = (width << 10) / ((s16)hStart - ((s32)hStart >> 16));
    if (old[0].h.end < v[0].h.end) {
        v[0].h.end = old[0].h.end;
    }
    if (old[1].h.end < v[1].h.end) {
        v[1].h.end = old[1].h.end;
    }
    if (oldRight < (s16)hStart) {
        hStart = (hStart & 0xFFFF0000) | (u16)oldHStart;
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
