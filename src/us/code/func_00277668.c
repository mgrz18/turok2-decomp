/* Draw the outline of a box, `border` pixels thick, in one flat colour: the top, bottom, left and right bars as fill rectangles. */
/* Same display-list style as its neighbour func_002778A8 (after Turok: Rage Wars' rectangle()). */

#include "common.h"

typedef union Gfx {
    struct { u32 w0; u32 w1; } words;
    u64 force_structure_alignment;
} Gfx;

extern Gfx *D_800D8E24;

#define _SHIFTL(v, s, w) (((u32) (v) & ((1U << (w)) - 1)) << (s))
#define CLAMP0(v) ((v) > 0 ? (v) : 0)

#define gDPPipeSync()                                                                      \
    { Gfx *_g = (Gfx *) (D_800D8E24++); _g->words.w0 = _SHIFTL(0xE7, 24, 8); _g->words.w1 = 0; }
#define gDPSetPrimColor(m, l, r, g, b, a)                                                  \
    { Gfx *_g = (Gfx *) (D_800D8E24++);                                                    \
      _g->words.w0 = _SHIFTL(0xFA, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8);           \
      _g->words.w1 = _SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8); }
#define gDPFillRectangle(ulx, uly, lrx, lry)                                               \
    { Gfx *_g = (Gfx *) (D_800D8E24++);                                                    \
      _g->words.w0 = _SHIFTL(0xF6, 24, 8) | _SHIFTL(lrx, 14, 10) | _SHIFTL(lry, 2, 10);    \
      _g->words.w1 = _SHIFTL(ulx, 14, 10) | _SHIFTL(uly, 2, 10); }

void func_00277668(s32 x0, s32 y0, s32 x1, s32 y1, s32 border, u8 r, u8 g, u8 b, u8 a) {
    gDPPipeSync();
    gDPSetPrimColor(255, 255, r, g, b, a);
    gDPFillRectangle(CLAMP0(x0), CLAMP0(y0), CLAMP0(x1), CLAMP0(y0 + border));
    gDPFillRectangle(CLAMP0(x0), CLAMP0(y1 - border), CLAMP0(x1), CLAMP0(y1));
    gDPFillRectangle(CLAMP0(x0), CLAMP0(y0 + border), CLAMP0(x0 + border), CLAMP0(y1 - border));
    gDPFillRectangle(CLAMP0(x1 - border), CLAMP0(y0 + border), CLAMP0(x1), CLAMP0(y1 - border));
}
