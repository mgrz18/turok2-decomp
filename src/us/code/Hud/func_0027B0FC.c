/* Draw a 16x16 texture as a texture rectangle at (x, y), scaled by (sx, sy): set the image and run the
 * display list sHudGlyph16x16LoadDL that loads it (the twin of Hud_DrawGlyph16x8 for the bigger glyphs). */

#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "globals/hud.h"

void Hud_DrawGlyph16x16(sptr texture, f32 x, f32 y, f32 sx, f32 sy) {
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
    yh = (y + sy * 16.0f) * 4.0f;
    xl = x * 4.0f;
    yl = y * 4.0f;
    g = gDList++;
    g->w0 = 0xFD900000;         /* gDPSetTextureImage(I, 16b, 1, texture) */
    g->w1 = texture;
    g = gDList++;
    g->w0 = 0xDE000000;         /* gSPDisplayList */
    g->w1 = GBI_ADDR(sHudGlyph16x16LoadDL);
    g = gDList++;           /* gSPTextureRectangle */
    g->w0 = 0xE4000000 | ((xh & 0xFFF) << 12) | (yh & 0xFFF);
    g->w1 = ((xl & 0xFFF) << 12) | (yl & 0xFFF);
    g = gDList++;
    g->w0 = 0xE1000000;
    g->w1 = 0;
    g = gDList++;
    g->w0 = 0xF1000000;
    g->w1 = (dsdx << 16) | (dtdy & 0xFFFF);
}
