/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/GFX/TEXT/802A8A94_3.CPP, func_802A9734_de, with Turok 2's layouts. */
/* Draw a 16-bit RGBA image through a 4-bit intensity mask as a scaled texture rectangle at a sub-pixel
 * position (x, y): the rectangle is clipped at the screen's top-left edge, the texture start shifted to
 * match, and the texture steps divided by the scales. */
#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"

void Hud_DrawMaskedBlockScaled(u16 *image, u8 *mask, f32 x, f32 y, s32 texWidth, s32 texHeight, f32 scaleX, f32 scaleY) {
    int dsdx = 1024, dtdy;
    int xl, yl, xh, yh;

    if (scaleX != 0.0f) {
        dsdx = dsdx / scaleX;
    }
    dtdy = 1024;
    if (scaleY != 0.0f) {
        dtdy = dtdy / scaleY;
    }
    xl = x * 4.0f;
    xh = (x + texWidth * scaleX) * 4.0f;
    yl = y * 4.0f;
    yh = (y + texHeight * scaleY) * 4.0f;

    gDPLoadMultiBlock_4b(gDList++, mask, 0x100, 0, G_IM_FMT_I, texWidth, texHeight, 0, G_TX_WRAP, G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTextureBlock(gDList++, image, G_IM_FMT_RGBA, G_IM_SIZ_16b, texWidth, texHeight, 0, G_TX_WRAP, G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
    gDPSetTile(gDList++, G_IM_FMT_I, G_IM_SIZ_4b, texWidth / 16, 0x100, 1, 0, 0, 0, 0, 0, 0, 0);
    gDPSetTileSize(gDList++, 1, 0, 0, (texWidth - 1) << 2, (texHeight - 1) << 2);
    gSPScisTextureRectangle(gDList++, xl, yl, xh, yh, 0, 0, 0, dsdx, dtdy);
}
