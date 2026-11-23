/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/GFX/TEXT/802A8A94.CPP, func_802A84F8_de, with Turok 2's layouts. */
/* Set up the font texture: two-cycle mode, combiner and render mode, then load the texture COnScreen__GetFontTexture
 * returns as an I texture (the 4-bit render tile 0 at 24x24 and tile 1 at 16x16), and reset the font
 * colour, scale and spacing. */

#include "common.h"
#include "gfx.h"
#include "globals/menu.h"

extern s32 gEngineApp_m_OnScreen_m_ShadowXOff[2]; /* local type: the field's real type (or the member path) changes the ROM's code here */       /* font spacing */

extern sptr COnScreen__GetFontTexture(void);
extern void Render_SetCombineMode(s32 mode);
extern void Render_SetRenderMode(s32 mode);
extern void COnScreen__SetFontColor(s32 r0, s32 g0, s32 b0, s32 r1, s32 g1, s32 b1);
extern void COnScreen__SetFontScale(f32 x, f32 y);

void COnScreen__BeginText(void) {
    sptr texture;

    texture = COnScreen__GetFontTexture();
    if (texture != 0) {
        {
            Gfx *g;
            g = gDList++;
            g->w0 = 0xE7000000;     /* gDPPipeSync */
            g->w1 = 0;
            g = gDList++;
            g->w0 = 0xE3000A01;     /* gDPSetCycleType(G_CYC_2CYCLE) */
            g->w1 = 0x00100000;
        }

        Render_SetCombineMode(0x1A);
#ifdef TARGET_PORT
        /* Mode 0x1A is ((ENV - PRIM) * TEXEL1 + PRIM, TEXEL0 * ENV alpha) in two cycles, and tile 1
         * (below) has line 0. The RDP's texel address is tmem + t * line + s, so every row of tile 1
         * is row 0 of the font block, 16 texels of 0xF: TEXEL1 is 1 and the text is the environment
         * colour. Fast3D makes an empty texture of a line-0 tile (TEXEL1 0): the text came out in the
         * dark primitive colour (docs/port/FIDELITY.md 2.3). The same colour on the host: ENV for
         * the colour, the alpha unchanged (round 11 z5). */
        {
            Gfx *g = gDList++;
            g->w0 = 0xFCFF9BFF;     /* gDPSetCombineLERP(0, 0, 0, ENV, TEXEL0, 0, ENV, 0, 0, 0, 0, COMBINED, ...) */
            g->w1 = 0xFFFEFE38;
        }
#endif
        Render_SetRenderMode(0x15);

        gLoadedFont = 0;
        {
            Gfx *g;
            g = gDList++;
            g->w0 = 0xD7000002;     /* gSPTexture(0x8000, 0x8000, 0, 0, G_ON) */
            g->w1 = 0x80008000;
            g = gDList++;
            g->w0 = 0xE3001001;     /* gDPSetTextureLUT(G_TT_NONE) */
            g->w1 = 0;
            g = gDList++;
            g->w0 = 0xE3000C00;     /* gDPSetTexturePersp(G_TP_NONE) */
            g->w1 = 0;
            g = gDList++;
            g->w0 = 0xE3001201;     /* gDPSetTextureFilter(G_TF_BILERP) */
            g->w1 = 0x2000;
            g = gDList++;
            g->w0 = 0xFD900000;     /* gDPSetTextureImage(I, 16b, 1, texture) */
            g->w1 = texture;
            g = gDList++;
            g->w0 = 0xF5900000;     /* gDPSetTile(I, 16b, 0, 0, G_TX_LOADTILE, ...) */
            g->w1 = 0x07000000;
            g = gDList++;
            g->w0 = 0xE6000000;     /* gDPLoadSync */
            g->w1 = 0;
            g = gDList++;
            g->w0 = 0xF3000000;     /* gDPLoadBlock(G_TX_LOADTILE, 0, 0, 143, 2048) */
            g->w1 = 0x0708F800;
            g = gDList++;
            g->w0 = 0xE7000000;     /* gDPPipeSync */
            g->w1 = 0;
            g = gDList++;
            g->w0 = 0xF5800400;     /* gDPSetTile(I, 4b, 2, 0, G_TX_RENDERTILE, ...) */
            g->w1 = 0;
            g = gDList++;
            g->w0 = 0xF2000000;     /* gDPSetTileSize(G_TX_RENDERTILE, 0, 0, 92, 92) */
            g->w1 = 0x0005C05C;
            g = gDList++;
            g->w0 = 0xF5800000;     /* gDPSetTile(I, 4b, 0, 0, 1, ...) */
            g->w1 = 0x01000000;
            g = gDList++;
            g->w0 = 0xF2000000;     /* gDPSetTileSize(1, 0, 0, 60, 60) */
            g->w1 = 0x0103C03C;
        }

        COnScreen__SetFontColor(0xC8, 0xC8, 0x8A, 0x56, 0x47, 0x2F);
        COnScreen__SetFontScale(1.0f, 1.0f);
        gEngineApp_m_OnScreen_m_ShadowXOff[0] = 2;
        gEngineApp_m_OnScreen_m_ShadowXOff[1] = 2;
    }
}
