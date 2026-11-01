/* NON-MATCHING: 169 of 185 words differ (W batch). */
/* NON-MATCHING: draft, see the report: the ROM keeps both fields' vStart words on the stack and edits them as s16
 * halves (sh/lh at sp+0x10..0x22), which this plain-integer version does not reproduce. */
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

extern s16 D_801309D0; /* selected VI mode */
extern ViMode D_801309D8;
extern ViMode D_800B7BE0[]; /* osViModeTable */
extern void *func_0029E460(void *dst, const void *src, s32 count);
extern void func_0029AA80(ViMode *mode); /* osViSetMode */

void func_00424008(s32 width, s32 height, s32 x, s32 y, s32 borderX, s32 borderY) {
    ViMode *mode = &D_801309D8;
    s32 centerX, centerY0, centerY1;
    s32 left, right, top0, bottom0, top1, bottom1;
    s32 oldRight, oldBottom0, oldBottom1;
    u32 scaleY;

    func_0029E460(mode, &D_800B7BE0[D_801309D0], 0x50);
    oldRight = (s16)mode->comRegs.hStart;
    oldBottom0 = (s16)mode->fldRegs[0].vStart;
    oldBottom1 = (s16)mode->fldRegs[1].vStart;
    centerX = ((oldRight - ((s32)mode->comRegs.hStart >> 16)) / 2 + ((s32)mode->comRegs.hStart >> 16)) - x;
    centerY0 = (mode->fldRegs[0].vStart >> 16) + (oldBottom0 - (s16)(mode->fldRegs[0].vStart >> 16)) / 2 - y;
    centerY1 = (mode->fldRegs[1].vStart >> 16) + (oldBottom1 - (s16)(mode->fldRegs[1].vStart >> 16)) / 2 - y;
    left = (s16)(centerX - width / 2 + borderX / 2);
    right = (s16)(centerX + width / 2 - borderX * 2);
    top0 = (s16)(centerY0 - height / 2 + borderY / 2);
    bottom0 = (s16)(centerY0 + height / 2 - borderY * 2);
    top1 = (s16)(centerY1 - height / 2 + borderY / 2);
    bottom1 = (s16)(centerY1 + height / 2 - borderY * 2 - 3);
    mode->comRegs.xScale = (width << 10) / (right - left);
    scaleY = (mode->fldRegs[0].yScale & 0xFFFF0000) | (((height << 11) / (bottom0 - top0)) & 0xFFFF);
    bottom0 = (s16)(bottom0 - 3);
    if (oldBottom0 < bottom0) {
        bottom0 = oldBottom0;
    }
    if (oldBottom1 < bottom1) {
        bottom1 = oldBottom1;
    }
    if (oldRight < right) {
        right = oldRight;
    }
    mode->fldRegs[0].yScale = scaleY;
    mode->fldRegs[1].yScale = scaleY;
    mode->fldRegs[0].origin = 0;
    mode->fldRegs[1].origin = width * 2;
    mode->comRegs.width = width;
    mode->comRegs.hStart = ((u32)left << 16) | (u16)right;
    mode->fldRegs[0].vStart = ((u32)top0 << 16) | (u16)bottom0;
    mode->fldRegs[1].vStart = ((u32)top1 << 16) | (u16)bottom1;
    func_0029AA80(mode);
}
