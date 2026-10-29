#include "common.h"
#include "gfx.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ s32 colorImage;
} GfxTask;

extern s32 D_800C2030;      /* screen width */
extern s32 D_80130A78;      /* fill target (z-buffer) */
extern GfxTask *D_800F7070[]; /* current graphics task */

extern void func_00207FEC(s32 mode);

/* Fills the rectangle (x0, y0)-(x1, y1) of the z-buffer, then points the color image back at the frame. */
void func_0028852C(s32 unused, s32 x0, s32 x1, s32 y0, s32 y1) {
    s32 colorImage;

    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    func_00207FEC(0x14);
    colorImage = 0xFF100000 | ((D_800C2030 - 1) & 0xFFF);
    {
        Gfx *g = D_800D8E24++;
        g->w0 = colorImage;     /* gDPSetColorImage(RGBA, 16b, width, z-buffer) */
        g->w1 = D_80130A78;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_FILL) */
        g->w1 = 0x00300000;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xF7000000;     /* gDPSetFillColor(max depth) */
        g->w1 = 0xFFFCFFFC;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xF6000000 | ((x1 & 0x3FF) << 14) | ((y1 & 0x3FF) << 2); /* gDPFillRectangle */
        g->w1 = ((x0 & 0x3FF) << 14) | ((y0 & 0x3FF) << 2);
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = colorImage;     /* gDPSetColorImage(RGBA, 16b, width, frame) */
        g->w1 = D_800F7070[0]->colorImage;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_1CYCLE) */
        g->w1 = 0x00100000;
    }
}
