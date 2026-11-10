#include "common.h"
#include "gfx.h"
#include "structs/game.h"
#include "globals/frame.h"
#include "globals/render.h"


extern void Render_SetRenderMode(s32 mode);

/* Fills the rectangle (x0, y0)-(x1, y1) of the z-buffer, then points the color image back at the frame. */
void CEngineApp__ClearDepthRect(sptr unused, s32 x0, s32 x1, s32 y0, s32 y1) {
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
