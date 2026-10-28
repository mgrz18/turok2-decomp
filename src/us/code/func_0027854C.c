/* Draw a 16x8 I4 texture (a font glyph strip) as a texture rectangle at (x, y), scaled by (sx, sy). */

#include "common.h"
#include "gfx.h"

void func_0027854C(s32 texture, f32 x, f32 y, f32 sx, f32 sy) {
    Gfx *g;
    s32 xh, yh, xl, yl;
    s32 dsdx;
    s32 dtdy;

    dsdx = 1 << 10;
    if (sx != 0.0f) {
        dsdx = (1.0f / sx) * dsdx;
    }
    dtdy = 1 << 10;
    if (sy != 0.0f) {
        dtdy = (1.0f / sy) * dtdy;
    }
    xh = (x + sx * 16.0f) * 4.0f;
    yh = (y + sy * 8.0f) * 4.0f;
    xl = x * 4.0f;
    yl = y * 4.0f;
    g = D_800D8E24++;
    g->w0 = 0xFD900000;         /* gDPSetTextureImage(I, 16b, 1, texture) */
    g->w1 = texture;
    g = D_800D8E24++;
    g->w0 = 0xF5900000;         /* gDPSetTile(I, 16b, 0, 0, G_TX_LOADTILE, ...) */
    g->w1 = 0x0708C240;
    g = D_800D8E24++;
    g->w0 = 0xE6000000;         /* gDPLoadSync */
    g->w1 = 0;
    g = D_800D8E24++;
    g->w0 = 0xF3000000;         /* gDPLoadBlock(G_TX_LOADTILE, 0, 0, 31, 2048) */
    g->w1 = 0x0701F800;
    g = D_800D8E24++;
    g->w0 = 0xE7000000;         /* gDPPipeSync */
    g->w1 = 0;
    g = D_800D8E24++;
    g->w0 = 0xF5800200;         /* gDPSetTile(I, 4b, 1, 0, G_TX_RENDERTILE, ...) */
    g->w1 = 0x0008C240;
    g = D_800D8E24++;
    g->w0 = 0xF2000000;         /* gDPSetTileSize(G_TX_RENDERTILE, 0, 0, 60, 28) */
    g->w1 = 0x0003C01C;
    g = D_800D8E24++;           /* gSPTextureRectangle */
    g->w0 = 0xE4000000 | ((xh & 0xFFF) << 12) | (yh & 0xFFF);
    g->w1 = ((xl & 0xFFF) << 12) | (yl & 0xFFF);
    g = D_800D8E24++;
    g->w0 = 0xE1000000;
    g->w1 = 0;
    g = D_800D8E24++;
    g->w0 = 0xF1000000;
    g->w1 = (dsdx << 16) | (dtdy & 0xFFFF);
}
