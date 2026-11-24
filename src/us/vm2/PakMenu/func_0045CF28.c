/* The second VM module's copy of src/us/vm1/PakMenu/func_00424008.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
/* xScale is a scratch first: it holds the new horizontal centre the edges are built from, then the scale. That second job is what keeps oldH canonical for CSE (a single-use copy is propagated away, and without one the centre is read from h's load). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/FRONTEND/8040B45C_21.CPP, func_8040BDB8_de, with Turok 2's layouts. */
#include "common.h"
#include "globals/libultra.h"
#include "globals/render.h"
#include "protos/ultra.h"
#include "structs/twin_pakmenu.h"

/* Resizes the selected VI mode around its original horizontal and field centres, keeping the original lower
 * screen limits, then applies it. */

/* A packed pair register (hStart, vStart, yScale): the high half and the low half. */





extern ViMode sResizedViMode;
extern void *func_0029E460(void *dst, const void *src, s32 count);

void Video_ResizeViMode_2(s32 width, s32 height, s32 x, s32 y, s32 borderX, s32 borderY) {
    ViMode *mode = &sResizedViMode;
    Span v[2];
    Span old[2];
    s16 center[2];
    Span h, oldH;
    Span yScale;
    u32 xScale;
    s32 oldRight;
    s32 cx;

    func_0029E460(mode, &osViModeTable[sViModeIndex], 0x50);
    old[0] = v[0] = mode->fldRegs[0].vStart;
    old[1] = v[1] = mode->fldRegs[1].vStart;
    center[0] = v[0].h.start + (v[0].h.end - v[0].h.start) / 2;
    center[1] = v[1].h.start + (v[1].h.end - v[1].h.start) / 2;
    h = mode->comRegs.hStart; /* new left/right edges */
    center[0] -= y;
    center[1] -= y;
    oldH = h; /* original edges */
    oldRight = oldH.h.end;
    cx = (oldRight - oldH.h.start) / 2 + oldH.h.start;
    cx -= x;
    xScale = cx;
    h.h.start = xScale - width / 2 + borderX / 2;
    h.h.end = xScale + width / 2 - borderX * 2;
    v[0].h.start = center[0] - height / 2 + borderY / 2;
    v[0].h.end = center[0] + height / 2 - borderY * 2;
    v[1].h.start = center[1] - height / 2 + borderY / 2;
    v[1].h.end = center[1] + height / 2 - borderY * 2;
    yScale = mode->fldRegs[0].yScale;
    yScale.h.end = (height << 11) / (v[0].h.end - v[0].h.start);
    v[0].h.end -= 3;
    v[1].h.end -= 3;
    xScale = (width << 10) / (h.h.end - h.h.start);
    if (old[0].h.end < v[0].h.end) {
        v[0].h.end = old[0].h.end;
    }
    if (old[1].h.end < v[1].h.end) {
        v[1].h.end = old[1].h.end;
    }
    if (oldRight < h.h.end) {
        h.h.end = oldH.h.end;
    }
    mode->fldRegs[0].yScale = yScale;
    mode->fldRegs[1].yScale = yScale;
    mode->fldRegs[0].origin = 0;
    mode->fldRegs[1].origin = width * 2;
    mode->comRegs.width = width;
    mode->comRegs.xScale = xScale;
    mode->comRegs.hStart = h;
    mode->fldRegs[0].vStart = v[0];
    mode->fldRegs[1].vStart = v[1];
    osViSetMode(mode);
}
