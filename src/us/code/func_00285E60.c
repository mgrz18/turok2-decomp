#include "common.h"
#include "gfx.h"
#include "structs/frame.h"

extern s32 D_800C2030;              /* screen width */
extern s32 D_800C2034;              /* screen height */

extern void CEngineApp__ClearDepthRect(s32, s32, s32, s32, s32);
extern void Hud_GetSpriteSize(s32, s32, s32 *, s32 *);
extern void Hud_SetupTextureMode(s32);
extern void Hud_DrawSprite(s32, s32, s32, s32, f32, f32, s32);

void EngineMode07_Title_Draw(s32 arg0) {
    CFrameData *fb;
    s32 width;
    s32 height;
    Gfx *g;

    fb = D_800F7070;
    CEngineApp__ClearDepthRect(arg0, 0, D_800C2030, 0, D_800C2034);
    width = 0;
    height = 0;
    /* gDPSetScissor(G_SC_NON_INTERLACE, 0, 0, w - 1, h - 1) */
    g = D_800D8E24++;
    g->w0 = 0xED000000;
    g->w1 = (((s32)((f32)(D_800C2030 - 1) * 4.0f) & 0xFFF) << 12) | ((s32)((f32)(D_800C2034 - 1) * 4.0f) & 0xFFF);
    /* gDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, w, depth buffer) */
    g = D_800D8E24++;
    g->w0 = 0xFF100000 | ((D_800C2030 - 1) & 0xFFF);
    g->w1 = fb->m_pFrameBuffer;
    /* gDPSetCycleType(G_CYC_FILL) */
    g = D_800D8E24++;
    g->w0 = 0xE3000A01;
    g->w1 = 0x300000;
    /* gDPSetCombineMode(G_CC_SHADE, G_CC_SHADE) */
    g = D_800D8E24++;
    g->w0 = 0xFCFFFFFF;
    g->w1 = 0xFFFE793C;
    /* gDPSetRenderMode(0, 0) */
    g = D_800D8E24++;
    g->w0 = 0xE200001C;
    g->w1 = 0;
    /* gDPSetFillColor: max depth */
    g = D_800D8E24++;
    g->w0 = 0xF7000000;
    g->w1 = 0x10001;
    /* gDPFillRectangle(0, 0, w, h) */
    g = D_800D8E24++;
    g->w0 = 0xF6000000 | ((D_800C2030 & 0x3FF) << 14) | ((D_800C2034 & 0x3FF) << 2);
    g->w1 = 0;
    Hud_GetSpriteSize(0x386, 0, &width, &height);
    if (width != 0 && height != 0) {
        Hud_SetupTextureMode(0xFF);
        Hud_DrawSprite(0x386, 0, 0, 0, (f32)D_800C2030 / (f32)width, (f32)D_800C2034 / (f32)height, 1);
    }
}
