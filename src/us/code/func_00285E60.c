#include "common.h"
#include "gfx.h"
#include "structs/frame.h"
#include "globals/render.h"


extern void CEngineApp__ClearDepthRect(s32, s32, s32, s32, s32);
extern void Hud_GetSpriteSize(s32, s32, s32 *, s32 *);
extern void Hud_SetupTextureMode(s32);
extern void Hud_DrawSprite(s32, s32, s32, s32, f32, f32, s32);

void EngineMode07_Title_Draw(s32 arg0) {
    CFrameData *fb;
    s32 width;
    s32 height;
    Gfx *g;

    fb = gEngineApp_m_pCurrentFrameData;
    CEngineApp__ClearDepthRect(arg0, 0, gScreenWidth, 0, gScreenHeight);
    width = 0;
    height = 0;
    /* gDPSetScissor(G_SC_NON_INTERLACE, 0, 0, w - 1, h - 1) */
    g = gDList++;
    g->w0 = 0xED000000;
    g->w1 = (((s32)((f32)(gScreenWidth - 1) * 4.0f) & 0xFFF) << 12) | ((s32)((f32)(gScreenHeight - 1) * 4.0f) & 0xFFF);
    /* gDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, w, depth buffer) */
    g = gDList++;
    g->w0 = 0xFF100000 | ((gScreenWidth - 1) & 0xFFF);
    g->w1 = fb->m_pFrameBuffer;
    /* gDPSetCycleType(G_CYC_FILL) */
    g = gDList++;
    g->w0 = 0xE3000A01;
    g->w1 = 0x300000;
    /* gDPSetCombineMode(G_CC_SHADE, G_CC_SHADE) */
    g = gDList++;
    g->w0 = 0xFCFFFFFF;
    g->w1 = 0xFFFE793C;
    /* gDPSetRenderMode(0, 0) */
    g = gDList++;
    g->w0 = 0xE200001C;
    g->w1 = 0;
    /* gDPSetFillColor: max depth */
    g = gDList++;
    g->w0 = 0xF7000000;
    g->w1 = 0x10001;
    /* gDPFillRectangle(0, 0, w, h) */
    g = gDList++;
    g->w0 = 0xF6000000 | ((gScreenWidth & 0x3FF) << 14) | ((gScreenHeight & 0x3FF) << 2);
    g->w1 = 0;
    Hud_GetSpriteSize(0x386, 0, &width, &height);
    if (width != 0 && height != 0) {
        Hud_SetupTextureMode(0xFF);
        Hud_DrawSprite(0x386, 0, 0, 0, (f32)gScreenWidth / (f32)width, (f32)gScreenHeight / (f32)height, 1);
    }
}
