/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/render_screen_overlay.CPP, Render_DrawTextLines, with Turok 2's layouts. */
#include "common.h"
#include "gfx.h"
#include "structs/game.h"
#include "globals/frame.h"

extern s32 gScreenWidth;      /* screen width */
extern s32 gScreenHeight;      /* screen height */
extern s32 gEngineApp_m_OnScreen_m_ShadowXOff[2];

extern void Render_SetRenderMode(s32 mode);
extern void COnScreen__BeginText(void);
extern void COnScreen__SelectSmallFont();
extern void COnScreen__SetFontScale(f32 x, f32 y);
extern s32 COnScreen__DrawString(char *text, s32 x, s32 y, s32 alpha, s32, s32, f32, f32);

/* CEngineApp__ClearDepthRect: fills a rectangle of the z-buffer, then points the color image back at the frame. */
static __inline__ void clearDepth(void *unused, s32 x0, s32 x1, s32 y0, s32 y1) {
    s32 colorImage;

    {
        Gfx *g = gDList++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    Render_SetRenderMode(0x14);
    colorImage = 0xFF100000 | ((gScreenWidth - 1) & 0xFFF);
    {
        Gfx *g = gDList++;
        g->w0 = colorImage;     /* gDPSetColorImage(RGBA, 16b, width, z-buffer) */
        g->w1 = gDepthBuffer;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_FILL) */
        g->w1 = 0x00300000;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xF7000000;     /* gDPSetFillColor(max depth) */
        g->w1 = 0xFFFCFFFC;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xF6000000 | ((x1 & 0x3FF) << 14) | ((y1 & 0x3FF) << 2); /* gDPFillRectangle */
        g->w1 = ((x0 & 0x3FF) << 14) | ((y0 & 0x3FF) << 2);
    }
    {
        Gfx *g = gDList++;
        g->w0 = colorImage;     /* gDPSetColorImage(RGBA, 16b, width, frame) */
        g->w1 = gEngineApp.m_pCurrentFrameData->m_pFrameBuffer;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_1CYCLE) */
        g->w1 = 0x00100000;
    }
}

/* Draws a NULL-terminated list of scaled strings, over a cleared screen if asked. */
void Render_DrawTextLines(void *state, s32 drawBackdrop, char **lines, s32 x, s32 y, s32 lineStep, f32 scaleX, f32 scaleY) {
    CFrameData *task = gEngineApp.m_pCurrentFrameData;
    char *text;
    s32 i;

    y = y * gScreenHeight / 222;
    if (drawBackdrop) {
        clearDepth(state, 0, gScreenWidth, 0, gScreenHeight);
        {
            Gfx *g = gDList++;
            g->w0 = 0xED000000;     /* gDPSetScissor(0, 0, width - 1, height - 1) */
            g->w1 = (((s32)((f32)(gScreenWidth - 1) * 4.0f) & 0xFFF) << 12) | ((s32)((f32)(gScreenHeight - 1) * 4.0f) & 0xFFF);
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xFF100000 | ((gScreenWidth - 1) & 0xFFF);  /* gDPSetColorImage(frame) */
            g->w1 = task->m_pFrameBuffer;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_FILL) */
            g->w1 = 0x00300000;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xFCFFFFFF;     /* gDPSetCombineMode(shade) */
            g->w1 = 0xFFFE793C;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xE200001C;     /* gDPSetRenderMode(0, 0) */
            g->w1 = 0;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xF7000000;     /* gDPSetFillColor */
            g->w1 = 0x10001;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xF6000000 | ((gScreenWidth & 0x3FF) << 14) | ((gScreenHeight & 0x3FF) << 2); /* gDPFillRectangle */
            g->w1 = 0;
        }
    }
    COnScreen__BeginText();
    COnScreen__SelectSmallFont();
    scaleX = scaleX * gScreenWidth * (1.0f / 284.0f);
    scaleY = scaleY * gScreenHeight * (1.0f / 222.0f);
    scaleX *= (gScreenHeight >= 0xDF) ? 0.8f : 0.85f;
    scaleY *= 0.95f;
    COnScreen__SetFontScale(scaleX, scaleY);
    gEngineApp_m_OnScreen_m_ShadowXOff[0] = 1;
    gEngineApp_m_OnScreen_m_ShadowXOff[1] = 1;
    for (i = 0; lines[i] != NULL; i++) {
        text = lines[i];
        COnScreen__DrawString(text, x, y, 0xFF, 1, 1, 1.0f, 1.0f);
        y = y + lineStep * scaleY;
    }
}
