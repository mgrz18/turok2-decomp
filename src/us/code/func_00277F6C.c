/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/GFX/TEXT/802A8A94.CPP, func_802A84F8_de, with Turok 2's layouts. */
/* Set up the font texture: two-cycle mode, combiner and render mode, then load the texture func_0027B4EC
 * returns as an I texture (the 4-bit render tile 0 at 24x24 and tile 1 at 16x16), and reset the font
 * colour, scale and spacing. */

#include "common.h"
#include "gfx.h"

extern s32 D_800F56A8;          /* which font texture is loaded */
extern s32 D_801101B0[2];       /* font spacing */

extern s32 func_0027B4EC(void);
extern void func_00207A70(s32 mode);
extern void func_00207FEC(s32 mode);
extern void COnScreen__SetFontColor(s32 r0, s32 g0, s32 b0, s32 r1, s32 g1, s32 b1);
extern void COnScreen__SetFontScale(f32 x, f32 y);

void func_00277F6C(void) {
    s32 texture;

    texture = func_0027B4EC();
    if (texture != 0) {
        {
            Gfx *g;
            g = D_800D8E24++;
            g->w0 = 0xE7000000;     /* gDPPipeSync */
            g->w1 = 0;
            g = D_800D8E24++;
            g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_2CYCLE) */
            g->w1 = 0x00100000;
        }

        func_00207A70(0x1A);
        func_00207FEC(0x15);

        D_800F56A8 = 0;
        {
            Gfx *g;
            g = D_800D8E24++;
            g->w0 = 0xD7000002;     /* gSPTexture(0x8000, 0x8000, 0, 0, G_ON) */
            g->w1 = 0x80008000;
            g = D_800D8E24++;
            g->w0 = 0xE3001001;     /* gDPSetTextureLUT(G_TT_NONE) */
            g->w1 = 0;
            g = D_800D8E24++;
            g->w0 = 0xE3000C00;     /* gDPSetTexturePersp(G_TP_NONE) */
            g->w1 = 0;
            g = D_800D8E24++;
            g->w0 = 0xE3001201;     /* gDPSetTextureFilter(G_TF_BILERP) */
            g->w1 = 0x2000;
            g = D_800D8E24++;
            g->w0 = 0xFD900000;     /* gDPSetTextureImage(I, 16b, 1, texture) */
            g->w1 = texture;
            g = D_800D8E24++;
            g->w0 = 0xF5900000;     /* gDPSetTile(I, 16b, 0, 0, G_TX_LOADTILE, ...) */
            g->w1 = 0x07000000;
            g = D_800D8E24++;
            g->w0 = 0xE6000000;     /* gDPLoadSync */
            g->w1 = 0;
            g = D_800D8E24++;
            g->w0 = 0xF3000000;     /* gDPLoadBlock(G_TX_LOADTILE, 0, 0, 143, 2048) */
            g->w1 = 0x0708F800;
            g = D_800D8E24++;
            g->w0 = 0xE7000000;     /* gDPPipeSync */
            g->w1 = 0;
            g = D_800D8E24++;
            g->w0 = 0xF5800400;     /* gDPSetTile(I, 4b, 2, 0, G_TX_RENDERTILE, ...) */
            g->w1 = 0;
            g = D_800D8E24++;
            g->w0 = 0xF2000000;     /* gDPSetTileSize(G_TX_RENDERTILE, 0, 0, 92, 92) */
            g->w1 = 0x0005C05C;
            g = D_800D8E24++;
            g->w0 = 0xF5800000;     /* gDPSetTile(I, 4b, 0, 0, 1, ...) */
            g->w1 = 0x01000000;
            g = D_800D8E24++;
            g->w0 = 0xF2000000;     /* gDPSetTileSize(1, 0, 0, 60, 60) */
            g->w1 = 0x0103C03C;
        }

        COnScreen__SetFontColor(0xC8, 0xC8, 0x8A, 0x56, 0x47, 0x2F);
        COnScreen__SetFontScale(1.0f, 1.0f);
        D_801101B0[0] = 2;
        D_801101B0[1] = 2;
    }
}
