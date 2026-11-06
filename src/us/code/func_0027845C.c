/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A9700.c, func_002A9700.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "gfx.h"

extern s32 D_800F56A8;
void *COnScreen__GetFontTexture(void);
void COnScreen__SelectTinyFont(void) {
    void *tex = COnScreen__GetFontTexture();
    if (tex != ((void *)0)) {
        D_800F56A8 = 4;
        { Gfx *_g = D_800D8E24++; _g->w0 = (0xFD900000); _g->w1 = ((u32)tex); };
        { Gfx *_g = D_800D8E24++; _g->w0 = (0xF5900100); _g->w1 = (0x0700C040); };
        { Gfx *_g = D_800D8E24++; _g->w0 = (0xE6000000); _g->w1 = (0); };
        { Gfx *_g = D_800D8E24++; _g->w0 = (0xF3000000); _g->w1 = (0x0701F800); };
        { Gfx *_g = D_800D8E24++; _g->w0 = (0xE7000000); _g->w1 = (0); };
        { Gfx *_g = D_800D8E24++; _g->w0 = (0xF5800300); _g->w1 = (0x0000C040); };
        { Gfx *_g = D_800D8E24++; _g->w0 = (0xF2000000); _g->w1 = (0x0003C01C); };
    }
}
