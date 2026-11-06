/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00292318.c, func_00292318, with Turok 2's layouts. */
#include "common.h"
#include "structs/game.h"
#include "gfx.h"

extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */
extern s32 D_800C2040;          /* frames left to clear the screen */
extern s32 D_80130A78;          /* z-buffer */
extern s32 D_800B6D1C;
extern f32 gFrameIncrement;          /* frame time */
extern f32 D_800B6D48;          /* render time, seconds */
extern void (*D_800B6D80[][3])(CEngineApp *);
extern s32 gNextTicks;
extern u8 D_8011AD28[];       /* the RSP task queue */
extern s32 osSendMesg(void *queue, void *msg, s32 flags);

extern u64 osGetTime(void);     /* osGetTime */
extern void CCache__BeginDisplayList(s32, s32, CFrameData *);
extern void Texture_InvalidateCache(void);
extern void func_0020B554(void);
extern void CEngineApp__BeginDisplayList(CEngineApp *);
extern void DLights_SwapBuffers(void);
extern void Controller_UpdatePakInsertion(void);
extern void CCameraPool__Advance(void *);
extern void CEngineApp__UpdateControllers(CEngineApp *);
extern void CEngineApp__DrawFilledRect(CEngineApp *, u8 *rgba, s32 left, s32 top, s32 right, s32 bottom);
extern void Video_NextResolution(void);
extern void Render_SetRenderMode(s32 mode);
extern s32 Automap_IsShown(void *);
extern void Automap_Draw(void *);
extern s32 CCache__EndDisplayList(s32);
extern void CEngineApp__SubmitGfxTask(CEngineApp *, CFrameData *);
extern void CCache__GetFrameDelay(void);
extern void Cinema_Update(void);

/* CEngineApp__ClearDepthRect: fills a rectangle of the z-buffer, then points the color image back at the frame. */
static __inline__ void clearDepth(void *unused, s32 x0, s32 x1, s32 y0, s32 y1) {
    s32 colorImage;

    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    Render_SetRenderMode(0x14);
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
        g->w1 = gEngineApp.m_pCurrentFrameData->m_pFrameBuffer;
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

/* CEngineApp__DrawFade: darkens the whole screen by the level fade. */
static __inline__ void drawFade(CEngineApp *level) {
    u8 col[4];
    f32 t;
    s32 v;

    col[0] = 0;
    col[1] = 0;
    col[2] = 0;
    t = level->m_Settings.fade;
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
    CEngineApp__DrawFilledRect(level, col, 0, 0, D_800C2030, D_800C2034);
}

/* Draws one frame of the level and hands its display list to the RSP. */
void CEngineApp__Draw(CEngineApp *level) {
    u64 start;
    f32 inSpeed;
    f32 outSpeed;

    start = osGetTime();
    CCache__BeginDisplayList(0, D_800B6D1C, D_8011F140 = gEngineApp.m_pCurrentFrameData);
    Texture_InvalidateCache();
    func_0020B554();
    CEngineApp__BeginDisplayList(level);
    DLights_SwapBuffers();
    Controller_UpdatePakInsertion();
    CCameraPool__Advance(level->m_Game.m_CameraPool);
    CEngineApp__UpdateControllers(level);
    if (D_800B6D80[level->m_Settings.mode][0] != NULL) {
        D_800B6D80[level->m_Settings.mode][0](level);
    }
    if (D_8011F140->m_bLines == 0) {
        drawFade(level);
    }
    if (D_800C2040 != 0) {
        if (D_800C2040 == 0x14) {
            Video_NextResolution();
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
            g->w1 = D_8011F140->m_pFrameBuffer;
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
    D_8011F140->m_DisplayListSize = (u32)D_800D8E24 - (u32)D_8011F140->m_pDisplayList;
    if (level->m_Settings.mode == 0xD) {
        D_8011F140->m_bLines = Automap_IsShown(&level->m_Game.m_Automap);
    } else {
        D_8011F140->m_bLines = 0;
    }
    if (D_8011F140->m_bLines != 0) {
        D_800D8E24 = gEngineApp.m_pCurrentFrameData->m_pLineList;
        Automap_Draw(&level->m_Game.m_Automap);
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
        D_8011F140->m_LineListSize = (u32)D_800D8E24 - (u32)D_8011F140->m_pLineList;
    }
    D_8011F140->m_nPredictTicks = gNextTicks;
    D_800B6D48 = (f32)(((osGetTime() - start) * 64) / 3) * 0.000001f;   /* OS_CYCLES_TO_USEC */
    if (CCache__EndDisplayList(0) != 0) {
        CEngineApp__SubmitGfxTask(level, D_8011F140);
    } else {
        CCache__GetFrameDelay();
        osSendMesg(D_8011AD28, &D_8011F140->m_Msg, 1);
    }
    if (level->m_Settings.fadeMode != 0) {
        if (level->m_Settings.fastFade != 0) {
            inSpeed = 0.5f;
            outSpeed = 0.5f;
        } else {
            inSpeed = 0.1f;
            outSpeed = 0.1f;
        }
        switch (level->m_Settings.fadeMode) {
        case 2:
            if (level->m_Settings.fade > 0.0f) {
                level->m_Settings.fade -= outSpeed * gFrameIncrement;
                if (level->m_Settings.fade < 0.0f) {
                    level->m_Settings.fade = 0.0f;
                }
            } else {
                level->m_Settings.fade -= 1.0f;
            }
            if (level->m_Settings.fade < -4.0f) {
                s32 old = level->m_Settings.mode;
                level->m_Settings.mode = 0x11;
                level->m_Settings.fade = 0.0f;
                level->m_Settings.fadeMode = 1;
                level->m_Settings.prevMode = old;
            }
            break;
        case 1:
            level->m_Settings.fade += inSpeed * ((gFrameIncrement < 0.5f) ? 0.5f : gFrameIncrement);
            if (level->m_Settings.fade >= 1.0f) {
                level->m_Settings.fadeMode = 0;
                level->m_Settings.fade = 1.0f;
                level->m_Settings.fastFade = 0;
            }
            break;
        }
    }
    Cinema_Update();
}
