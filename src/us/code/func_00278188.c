#include "common.h"
#include "gfx.h"


extern Gfx *D_800D8E24;
extern s32 D_800F56A8;
s32 COnScreen__GetFontTexture(void);

/* Loads the texture COnScreen__GetFontTexture returns into TMEM, as gDPLoadTextureBlock
 * would: set the image, the load tile, sync, load the block, then the render
 * tile and its size (16x16). */
void COnScreen__SelectNormalFont(void) {
    Gfx *g;
    s32 tex = COnScreen__GetFontTexture();

    if (tex != 0) {
        D_800F56A8 = 0;
        g = D_800D8E24++;
        g->w0 = 0xFD900000;
        g->w1 = tex;
        g = D_800D8E24++;
        g->w0 = 0xF5900100;
        g->w1 = 0x07010040;
        g = D_800D8E24++;
        g->w0 = 0xE6000000;
        g->w1 = 0;
        g = D_800D8E24++;
        g->w0 = 0xF3000000;
        g->w1 = 0x0703F800;
        g = D_800D8E24++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
        g = D_800D8E24++;
        g->w0 = 0xF5800300;
        g->w1 = 0x00010040;
        g = D_800D8E24++;
        g->w0 = 0xF2000000;
        g->w1 = 0x0003C03C;
    }
}
