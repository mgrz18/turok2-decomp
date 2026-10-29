/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/render_screen_overlay.CPP, Render_DrawTextLines, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"

typedef struct GfxTask {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ s32 colorImage;
} GfxTask;

extern s32 D_800C2030;      /* screen width */
extern s32 D_800C2034;      /* screen height */
extern s32 D_80130A78;      /* z-buffer */
extern GfxTask *D_800F7070[]; /* current graphics task */
extern s32 D_801101B0[2];

extern void func_00207FEC(s32 mode);
extern void func_00277F6C(void);
extern void func_0027836C();
extern void COnScreen__SetFontScale(f32 x, f32 y);
extern s32 func_00278CDC(char *text, s32 x, s32 y, s32 alpha, s32, s32, f32, f32);

/* func_0028852C: fills a rectangle of the z-buffer, then points the color image back at the frame. */
static __inline__ void clearDepth(void *unused, s32 x0, s32 x1, s32 y0, s32 y1) {
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

/* Draws a NULL-terminated list of scaled strings, over a cleared screen if asked. */
void func_00286E14(void *state, s32 drawBackdrop, char **lines, s32 x, s32 y, s32 lineStep, f32 scaleX, f32 scaleY) {
    GfxTask *task = D_800F7070[0];
    char *text;
    s32 i;

    y = y * D_800C2034 / 222;
    if (drawBackdrop) {
        clearDepth(state, 0, D_800C2030, 0, D_800C2034);
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xED000000;     /* gDPSetScissor(0, 0, width - 1, height - 1) */
            g->w1 = (((s32)((f32)(D_800C2030 - 1) * 4.0f) & 0xFFF) << 12) | ((s32)((f32)(D_800C2034 - 1) * 4.0f) & 0xFFF);
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xFF100000 | ((D_800C2030 - 1) & 0xFFF);  /* gDPSetColorImage(frame) */
            g->w1 = task->colorImage;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_FILL) */
            g->w1 = 0x00300000;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xFCFFFFFF;     /* gDPSetCombineMode(shade) */
            g->w1 = 0xFFFE793C;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xE200001C;     /* gDPSetRenderMode(0, 0) */
            g->w1 = 0;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xF7000000;     /* gDPSetFillColor */
            g->w1 = 0x10001;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xF6000000 | ((D_800C2030 & 0x3FF) << 14) | ((D_800C2034 & 0x3FF) << 2); /* gDPFillRectangle */
            g->w1 = 0;
        }
    }
    func_00277F6C();
    func_0027836C();
    scaleX = scaleX * D_800C2030 * (1.0f / 284.0f);
    scaleY = scaleY * D_800C2034 * (1.0f / 222.0f);
    scaleX *= (D_800C2034 >= 0xDF) ? 0.8f : 0.85f;
    scaleY *= 0.95f;
    COnScreen__SetFontScale(scaleX, scaleY);
    D_801101B0[0] = 1;
    D_801101B0[1] = 1;
    for (i = 0; lines[i] != NULL; i++) {
        text = lines[i];
        func_00278CDC(text, x, y, 0xFF, 1, 1, 1.0f, 1.0f);
        y = y + lineStep * scaleY;
    }
}
