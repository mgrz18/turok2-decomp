/* Draw a 16-bit RGBA image through a 4-bit intensity mask (both 32x32-wrapping) as one texture
 * rectangle at (x, y), starting the texture at texel (s, t) and shortening the rectangle to match. */

#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"

void Hud_DrawMaskedBlockAt(u16 *image, u8 *mask, s32 x, s32 y, s32 width, s32 height, s32 s, s32 t) {
    gDPLoadMultiBlock_4b(gDList++, mask, 0x100, 0, G_IM_FMT_I, width, height, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTextureBlock(gDList++, image, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, height, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
    gDPSetTile(gDList++, G_IM_FMT_I, G_IM_SIZ_4b, width / 16, 0x100, 1, 0, 0, 0, 0, 0, 0, 0);
    gDPSetTileSize(gDList++, 1, 0, 0, (width - 1) << 2, (height - 1) << 2);
    gSPTextureRectangle(gDList++, x << 2, y << 2, (x + (width - s)) << 2, (y + (height - t)) << 2, 0, s << 5, t << 5, 1 << 10,
                        1 << 10);
}
