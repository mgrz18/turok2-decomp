/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00292318.c, func_00292318, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"

typedef struct GfxTask {
    /* 0x000 */ u8 pad0[0xF0];
    /* 0x0F0 */ u8 msg[0x20];
    /* 0x110 */ s32 colorImage;
    /* 0x114 */ u32 dlStart;
    /* 0x118 */ u32 dlStart2;
    /* 0x11C */ s32 dlSize;
    /* 0x120 */ s32 dlSize2;
    /* 0x124 */ s32 frame;
    /* 0x128 */ s32 hasOverlay;
} GfxTask;

typedef struct Level {
    /* 0x00000 */ u8 pad0[0x22C00];
    /* 0x22C00 */ u8 unk22C00[0x23DF0 - 0x22C00];
    /* 0x23DF0 */ u8 overlay[0x23FD4 - 0x23DF0];
    /* 0x23FD4 */ s32 prevState;
    /* 0x23FD8 */ s32 state;
    /* 0x23FDC */ u8 pad23FDC[4];
    /* 0x23FE0 */ u8 fastFade;
    /* 0x23FE1 */ u8 fadeMode;     /* 1 fading in, 2 fading out */
    /* 0x23FE2 */ u8 pad23FE2[2];
    /* 0x23FE4 */ f32 fade;
} Level;

extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */
extern s32 D_800C2040;          /* frames left to clear the screen */
extern s32 D_80130A78;          /* z-buffer */
extern GfxTask *D_800F7070[];   /* current graphics task */
extern GfxTask *D_8011F140;
extern s32 D_800B6D1C;
extern f32 D_800B6D28;          /* frame time */
extern f32 D_800B6D48;          /* render time, seconds */
extern void (*D_800B6D80[][3])(Level *);
extern s32 D_8011AD10;
extern u8 D_8011AD28[];       /* the RSP task queue */
extern s32 osSendMesg(void *queue, void *msg, s32 flags);

extern u64 func_0029B170(void);     /* osGetTime */
extern void func_0020548C(s32, s32, GfxTask *);
extern void func_00207A54(void);
extern void func_0020B554(void);
extern void func_00286A04(Level *);
extern void func_002288D0(void);
extern void func_00284CE0(void);
extern void func_0027ECF0(void *);
extern void func_002886AC(Level *);
extern void func_002883E0(Level *, u8 *rgba, s32 left, s32 top, s32 right, s32 bottom);
extern void func_004242EC(void);
extern void func_00207FEC(s32 mode);
extern s32 func_00401C1C(void *);
extern void func_004004D8(void *);
extern s32 func_00205574(s32);
extern void func_00286B58(Level *, GfxTask *);
extern void func_00206078(void);
extern void func_00283CD4(void);

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

/* func_00288B1C: darkens the whole screen by the level fade. */
static __inline__ void drawFade(Level *level) {
    u8 col[4];
    f32 t;
    s32 v;

    col[0] = 0;
    col[1] = 0;
    col[2] = 0;
    t = level->fade;
    if (t < 0.0f) goto opaque;
    if (t > 1.0f) {
        v = 0;
        goto done;
    }
    if (t < 0.0f) {
    opaque:
        v = 255;
    } else {
        v = ~(s32)(t * 255.0f);
    }
done:
    col[3] = v;
    func_002883E0(level, col, 0, 0, D_800C2030, D_800C2034);
}

/* Draws one frame of the level and hands its display list to the RSP. */
void func_00287248(Level *level) {
    u64 start;
    f32 inSpeed;
    f32 outSpeed;

    start = func_0029B170();
    func_0020548C(0, D_800B6D1C, D_8011F140 = D_800F7070[0]);
    func_00207A54();
    func_0020B554();
    func_00286A04(level);
    func_002288D0();
    func_00284CE0();
    func_0027ECF0(level->unk22C00);
    func_002886AC(level);
    if (D_800B6D80[level->state][0] != NULL) {
        D_800B6D80[level->state][0](level);
    }
    if (D_8011F140->hasOverlay == 0) {
        drawFade(level);
    }
    if (D_800C2040 != 0) {
        if (D_800C2040 == 0x14) {
            func_004242EC();
        }
        clearDepth(level, 0, 0x1E0, 0, 0x168);
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xED000000;     /* gDPSetScissor(0, 0, 479, 359) */
            g->w1 = 0x77C59C;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xFF1001DF;     /* gDPSetColorImage(frame, 480) */
            g->w1 = D_8011F140->colorImage;
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
            g->w0 = 0xF67805A0;     /* gDPFillRectangle(0, 0, 480, 360) */
            g->w1 = 0;
        }
        D_800C2040--;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE9000000;         /* gDPFullSync */
        g->w1 = 0;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xDF000000;         /* gSPEndDisplayList */
        g->w1 = 0;
    }
    D_8011F140->dlSize = (u32)D_800D8E24 - D_8011F140->dlStart;
    if (level->state == 0xD) {
        D_8011F140->hasOverlay = func_00401C1C(level->overlay);
    } else {
        D_8011F140->hasOverlay = 0;
    }
    if (D_8011F140->hasOverlay != 0) {
        D_800D8E24 = (Gfx *)D_800F7070[0]->dlStart2;
        func_004004D8(level->overlay);
        drawFade(level);
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xE9000000;     /* gDPFullSync */
            g->w1 = 0;
        }
        {
            Gfx *g = D_800D8E24++;
            g->w0 = 0xDF000000;     /* gSPEndDisplayList */
            g->w1 = 0;
        }
        D_8011F140->dlSize2 = (u32)D_800D8E24 - D_8011F140->dlStart2;
    }
    D_8011F140->frame = D_8011AD10;
    D_800B6D48 = (f32)(((func_0029B170() - start) * 64) / 3) * 0.000001f;   /* OS_CYCLES_TO_USEC */
    if (func_00205574(0) != 0) {
        func_00286B58(level, D_8011F140);
    } else {
        func_00206078();
        osSendMesg(D_8011AD28, D_8011F140->msg, 1);
    }
    if (level->fadeMode != 0) {
        if (level->fastFade != 0) {
            inSpeed = 0.5f;
            outSpeed = 0.5f;
        } else {
            inSpeed = 0.1f;
            outSpeed = 0.1f;
        }
        switch (level->fadeMode) {
        case 2:
            if (level->fade > 0.0f) {
                level->fade -= outSpeed * D_800B6D28;
                if (level->fade < 0.0f) {
                    level->fade = 0.0f;
                }
            } else {
                level->fade -= 1.0f;
            }
            if (level->fade < -4.0f) {
                s32 old = level->state;
                level->state = 0x11;
                level->fade = 0.0f;
                level->fadeMode = 1;
                level->prevState = old;
            }
            break;
        case 1:
            level->fade += inSpeed * ((D_800B6D28 < 0.5f) ? 0.5f : D_800B6D28);
            if (level->fade >= 1.0f) {
                level->fadeMode = 0;
                level->fade = 1.0f;
                level->fastFade = 0;
            }
            break;
        }
    }
    func_00283CD4();
}
