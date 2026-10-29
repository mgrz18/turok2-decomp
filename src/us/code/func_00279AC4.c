/* Draw a 16-bit RGBA image through a 4-bit intensity mask (both 32x32-wrapping) as one texture
 * rectangle at (x, y), starting the texture at texel (s, t) and shortening the rectangle to match. */

#include "common.h"

typedef union Gfx {
    struct { u32 w0; u32 w1; } words;
    u64 force_structure_alignment;
} Gfx;

extern Gfx *D_800D8E24;

/* What this needs of libultra's PR/gbi.h (F3DEX2), written to D_800D8E24. */
#define _SHIFTL(v, s, w) ((unsigned int) (((unsigned int) (v) & ((0x01 << (w)) - 1)) << (s)))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define G_TX_LDBLK_MAX_TXL 2047
#define TXL2WORDS(txls, b_txl) MAX(1, ((txls) * (b_txl) / 8))
#define CALC_DXT(width, b_txl) (((1 << 11) + TXL2WORDS(width, b_txl) - 1) / TXL2WORDS(width, b_txl))
#define TXL2WORDS_4b(txls) MAX(1, ((txls) / 16))
#define CALC_DXT_4b(width) (((1 << 11) + TXL2WORDS_4b(width) - 1) / TXL2WORDS_4b(width))

#define G_IM_FMT_RGBA 0
#define G_IM_FMT_I 4
#define G_IM_SIZ_4b 0
#define G_IM_SIZ_16b 2

#define gDPNoParam(cmd) \
    { Gfx *_g = (Gfx *) (D_800D8E24++); _g->words.w0 = _SHIFTL(cmd, 24, 8); _g->words.w1 = 0; }
#define gDPPipeSync() gDPNoParam(0xE7)
#define gDPLoadSync() gDPNoParam(0xE6)
#define gDPSetTextureImage(fmt, siz, width, i)                                                  \
    { Gfx *_g = (Gfx *) (D_800D8E24++);                                                         \
      _g->words.w0 = _SHIFTL(0xFD, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) |         \
                     _SHIFTL((width) - 1, 0, 12);                                               \
      _g->words.w1 = (unsigned int) (i); }
#define gDPSetTileM(fmt, siz, line, tmem, tile, maskt, masks)                                    \
    { Gfx *_g = (Gfx *) (D_800D8E24++);                                                         \
      _g->words.w0 = _SHIFTL(0xF5, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) |         \
                     _SHIFTL(line, 9, 9) | _SHIFTL(tmem, 0, 9);                                 \
      _g->words.w1 = _SHIFTL(tile, 24, 3) | _SHIFTL(maskt, 14, 4) | _SHIFTL(masks, 4, 4); }
#define gDPSetTile(fmt, siz, line, tmem, tile) gDPSetTileM(fmt, siz, line, tmem, tile, 0, 0)
#define gDPLoadBlock(tile, uls, ult, lrs, dxt)                                                  \
    { Gfx *_g = (Gfx *) (D_800D8E24++);                                                         \
      _g->words.w0 = (_SHIFTL(0xF3, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12));       \
      _g->words.w1 = (_SHIFTL(tile, 24, 3) | _SHIFTL((MIN(lrs, G_TX_LDBLK_MAX_TXL)), 12, 12) |  \
                      _SHIFTL(dxt, 0, 12)); }
#define gDPSetTileSize(t, uls, ult, lrs, lrt)                                                   \
    { Gfx *_g = (Gfx *) (D_800D8E24++);                                                         \
      _g->words.w0 = _SHIFTL(0xF2, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12);         \
      _g->words.w1 = _SHIFTL(t, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(lrt, 0, 12); }
#define gImmp1(c, p0) \
    { Gfx *_g = (Gfx *) (D_800D8E24++); _g->words.w0 = _SHIFTL((c), 24, 8); _g->words.w1 = (unsigned int) (p0); }
/* gDPLoadMultiBlock_4b(timg, tmem, rtile, fmt, width, height, 0, G_TX_WRAP, G_TX_WRAP, masks, maskt, 0, 0) */
#define gDPLoadMultiBlock_4b(timg, tmem, rtile, fmt, width, height, masks, maskt)               \
    {                                                                                           \
        gDPSetTextureImage(fmt, G_IM_SIZ_16b, 1, timg);                                         \
        gDPSetTileM(fmt, G_IM_SIZ_16b, 0, tmem, 7, maskt, masks);                               \
        gDPLoadSync();                                                                          \
        gDPLoadBlock(7, 0, 0, (((width) * (height) + 3) >> 2) - 1, CALC_DXT_4b(width));         \
        gDPPipeSync();                                                                          \
        gDPSetTileM(fmt, G_IM_SIZ_4b, ((((width) >> 1) + 7) >> 3), tmem, rtile, maskt, masks);  \
        gDPSetTileSize(rtile, 0, 0, ((width) - 1) << 2, ((height) - 1) << 2)                   \
    }

/* gDPLoadTextureBlock(timg, fmt, G_IM_SIZ_16b, width, height, 0, G_TX_WRAP, G_TX_WRAP, masks, maskt, 0, 0) */
#define gDPLoadTextureBlock_16b(timg, fmt, width, height, masks, maskt)                         \
    {                                                                                           \
        gDPSetTextureImage(fmt, G_IM_SIZ_16b, 1, timg);                                         \
        gDPSetTileM(fmt, G_IM_SIZ_16b, 0, 0, 7, maskt, masks);                                  \
        gDPLoadSync();                                                                          \
        gDPLoadBlock(7, 0, 0, ((width) * (height)) - 1, CALC_DXT(width, 2));                    \
        gDPPipeSync();                                                                          \
        gDPSetTileM(fmt, G_IM_SIZ_16b, (((width) * 2) + 7) >> 3, 0, 0, maskt, masks);           \
        gDPSetTileSize(0, 0, 0, ((width) - 1) << 2, ((height) - 1) << 2)                        \
    }


#define gSPTextureRectangle(xl, yl, xh, yh, tile, s, t, dsdx, dtdy)                             \
    { Gfx *_g = (Gfx *) (D_800D8E24++);                                                         \
      _g->words.w0 = (_SHIFTL(0xE4, 24, 8) | _SHIFTL(xh, 12, 12) | _SHIFTL(yh, 0, 12));         \
      _g->words.w1 = (_SHIFTL(tile, 24, 3) | _SHIFTL(xl, 12, 12) | _SHIFTL(yl, 0, 12));         \
      gImmp1(0xE1, (_SHIFTL(s, 16, 16) | _SHIFTL(t, 0, 16)));                                   \
      gImmp1(0xF1, (_SHIFTL(dsdx, 16, 16) | _SHIFTL(dtdy, 0, 16))); }

void func_00279AC4(u16 *image, u8 *mask, s32 x, s32 y, s32 width, s32 height, s32 s, s32 t) {
    gDPLoadMultiBlock_4b(mask, 0x100, 0, G_IM_FMT_I, width, height, 5, 5);
    gDPLoadTextureBlock_16b(image, G_IM_FMT_RGBA, width, height, 5, 5);
    gDPSetTile(G_IM_FMT_I, G_IM_SIZ_4b, width / 16, 0x100, 1);
    gDPSetTileSize(1, 0, 0, (width - 1) << 2, (height - 1) << 2);
    gSPTextureRectangle(x << 2, y << 2, (x + (width - s)) << 2, (y + (height - t)) << 2, 0, s << 5, t << 5, 1 << 10,
                        1 << 10);
}
