/* Draw a 16-bit RGBA image through a 4-bit intensity mask as one texture rectangle: the mask goes to
 * TMEM 0x100 (tile 1), the image to TMEM 0 (tile 0), both texWidth x texHeight, drawn at (x, y) with the
 * size (width, height) at one texel per pixel. */

#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"

void Hud_DrawMaskedBlockSized(u16 *image, u8 *mask, s32 x, s32 y, s32 texWidth, s32 texHeight, s32 width, s32 height) {
    gDPLoadMultiBlock_4b(gDList++, mask, 0x100, 0, G_IM_FMT_I, texWidth, texHeight, 0, G_TX_WRAP, G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTextureBlock(gDList++, image, G_IM_FMT_RGBA, G_IM_SIZ_16b, texWidth, texHeight, 0, G_TX_WRAP, G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
    gDPSetTile(gDList++, G_IM_FMT_I, G_IM_SIZ_4b, texWidth / 16, 0x100, 1, 0, 0, 0, 0, 0, 0, 0);
    gDPSetTileSize(gDList++, 1, 0, 0, (texWidth - 1) << 2, (texHeight - 1) << 2);
    gSPTextureRectangle(gDList++, x << 2, y << 2, (x + width) << 2, (y + height) << 2, 0, 0, 0, 1 << 10, 1 << 10);
}
