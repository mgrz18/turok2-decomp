/* COnScreen__DrawBossBar and COverlay__Update, one source file: splat's func_00277088
 * started with the two junk words before this object (versions/cuts.us.txt),
 * and the 1.0f at 0x800A8240 that opens the file's .rodata is
 * COnScreen__DrawBossBar's. */
#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/render.h"

extern s32 Cinema_PathPlaying(void);
void Hud_GetSpriteSize(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3);
void Hud_SetupFillMode(void);
void COverlay__Update(COverlay *ov);
void Hud_FillRect(s32 x0,s32 y0,s32 x1,s32 y1,u8 r,u8 g,u8 b,u8 a);
void Hud_SetupTextureMode(s32 color);
s32 Hud_DrawSprite(s32 arg0, s32 arg1, s16 arg2, s16 arg3, f32 arg4, f32 arg5, s32 arg6);

/* Draws a boss health bar (1: m_BossOverlay, green; 2: m_BossOverlay2, red):
 * the frame sprite 0x191 and a bar whose length eases toward the boss percent. */
void COnScreen__DrawBossBar(COnScreen *os, s32 which) {
    s32 width;
    s32 height;
    COverlay *ov;
    f32 percent;
    s32 x;
    s32 y;
    s32 r;
    s32 g;
    s32 b;
    s32 yOff;
    u8 *alpha;
    s32 left;

    if (Cinema_PathPlaying() != 0 || gEngineApp_m_Settings.mode != 13) {
        return;
    }
    Hud_GetSpriteSize(0x191, 0, &width, &height);
    if (width == 0 || height == 0) {
        return;
    }
    switch (which) {
        case 1:
            ov = &os->m_BossOverlay;
            percent = os->m_BossPercent;
            r = 0x32;
            g = 0xFF;
            b = 0x32;
            yOff = 8;
            break;
        case 2:
            ov = &os->m_BossOverlay2;
            percent = os->m_BossPercent2;
            r = 0xFF;
            g = 0x32;
            b = 0x32;
            yOff = 0x15;
            break;
        default:
            return;
    }
    x = ov->m_PositionX + (f32)((gScreenWidth - width) / 2);
    y = ov->m_PositionY + (f32)yOff;
    Hud_SetupFillMode();
    COverlay__Update(ov);
    if (ov->m_Mode == 1) {
        if (ov->m_Value < percent) {
            ov->m_Value += 8;
            if (percent < ov->m_Value) {
                ov->m_Value = percent;
            }
        } else if (percent < ov->m_Value) {
            ov->m_Value -= 4;
            if (ov->m_Value < percent) {
                ov->m_Value = percent;
            }
        }
    }
    alpha = &gEngineApp_m_Settings.hudAlpha;
    left = x + 0x1E;
    Hud_FillRect(left, y + 8, left + (width - 0x3C) * ov->m_Value / 100, y + 0xE,
                  r, g, b, *alpha * 200 / 255);
    Hud_SetupTextureMode(*alpha);
    Hud_DrawSprite(0x191, 0, x, y, 1.0f, 1.0f, 0);
}

/* Runs a HUD overlay's script (docs/HUD.md) and the motion it started, one
 * step a frame. Ops: 0 stop, 1 set position, 2 wait, 3 slide to, 4 fall in
 * Y, 5 fall in X, 6 end. Script values are whole numbers; the fall
 * accelerations are 16.16 fixed point. An op that takes no time runs the
 * next one at once. */
void COverlay__Update(COverlay *ov) {
    s32 next;
    f32 dx;
    f32 dy;
    s32 bottom = gScreenHeight + 0x60;
    s32 right = gScreenWidth + 0x60;

    do {
        next = 0;
        switch (ov->m_Mode) {
            case 0:
            case 1:
                break;
            case 2:
                if (ov->m_pScript == NULL) {
                    goto no_script;
                }
                switch (*ov->m_pScript++) {
                    case 6:
                        ov->m_Mode = 0;
                        ov->m_pScript = NULL;
                        break;
                    case 1:
                        next = 1;
                        ov->m_PositionX = *ov->m_pScript++;
                        ov->m_PositionY = *ov->m_pScript++;
                        break;
                    case 2:
                        ov->m_Mode = 3;
                        ov->m_Time = *ov->m_pScript++;
                        break;
                    case 3:
                        ov->m_bOnScreen = 1;
                        ov->m_Mode = 4;
                        ov->m_FinalX = *ov->m_pScript++;
                        ov->m_FinalY = *ov->m_pScript++;
                        ov->m_ZoomFactor = 1.0f / *ov->m_pScript++;
                        break;
                    case 4:
                        ov->m_bOnScreen = 1;
                        ov->m_Mode = 5;
                        ov->m_VelY = 0.0f;
                        ov->m_AccelY = *ov->m_pScript++ * (1.0f / 65536.0f);
                        break;
                    case 5:
                        ov->m_bOnScreen = 1;
                        ov->m_Mode = 6;
                        ov->m_VelX = 0.0f;
                        ov->m_AccelX = *ov->m_pScript++ * (1.0f / 65536.0f);
                        break;
                    default:
                        ov->m_Mode = 1;
                        ov->m_pScript = NULL;
                        break;
                }
                break;
            case 3:
                if (ov->m_Time-- <= 0) {
                    next = 1;
                }
                break;
            case 4:
                dx = (ov->m_FinalX - ov->m_PositionX) * ov->m_ZoomFactor;
                dy = (ov->m_FinalY - ov->m_PositionY) * ov->m_ZoomFactor;
                ov->m_PositionX += dx;
                ov->m_PositionY += dy;
                if (((dx < 0.0f) ? -dx : dx) < 0.25f && ((dy < 0.0f) ? -dy : dy) < 0.25f) {
                    ov->m_PositionX = ov->m_FinalX;
                    ov->m_PositionY = ov->m_FinalY;
                    next = 1;
                }
                break;
            case 5:
                ov->m_PositionY += ov->m_VelY;
                ov->m_VelY += ov->m_AccelY;
                if (bottom < ov->m_PositionY || ov->m_PositionY < -90.0f) {
                    ov->m_bOnScreen = 0;
                    next = 1;
                }
                break;
            case 6:
                ov->m_PositionX += ov->m_VelX;
                ov->m_VelX += ov->m_AccelX;
                if (right < ov->m_PositionX || ov->m_PositionX < -90.0f) {
                    ov->m_bOnScreen = 0;
                    next = 1;
                }
                break;
            no_script:              /* here, not inline in case 2: the ROM's order */
                ov->m_Mode = 1;
                break;
        }
        if (!next) {
            return;
        }
        ov->m_Mode = 2;
    } while (1);
}

#include "gfx.h"
#include "gbi_t2.h"
#define CLAMP0(v) ((v) > 0 ? (v) : 0)

void Hud_DrawBoxOutline(s32 x0, s32 y0, s32 x1, s32 y1, s32 border, u8 r, u8 g, u8 b, u8 a) {
    gDPPipeSync(gDList++);
    gDPSetPrimColor(gDList++, 255, 255, r, g, b, a);
    gDPFillRectangle(gDList++, CLAMP0(x0), CLAMP0(y0), CLAMP0(x1), CLAMP0(y0 + border));
    gDPFillRectangle(gDList++, CLAMP0(x0), CLAMP0(y1 - border), CLAMP0(x1), CLAMP0(y1));
    gDPFillRectangle(gDList++, CLAMP0(x0), CLAMP0(y0 + border), CLAMP0(x0 + border), CLAMP0(y1 - border));
    gDPFillRectangle(gDList++, CLAMP0(x1 - border), CLAMP0(y0 + border), CLAMP0(x1), CLAMP0(y1 - border));
}
void Hud_DrawBevelBox(s32 x0,s32 y0,s32 x1,s32 y1,s32 border,s32 invert,s32 red,s32 green,s32 blue,s32 alpha);

static __inline__ void rectangle(s32 x0,s32 y0,s32 x1,s32 y1,s32 r,s32 g,s32 b,s32 a) {
    { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
    { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((((unsigned int)(r) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(g) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(b) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(a) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(x1>0?x1:0) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(y1>0?y1:0) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->w1 = ((((unsigned int)(x0>0?x0:0) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(y0>0?y0:0) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
}

void Hud_DrawBevelBox(s32 x0,s32 y0,s32 x1,s32 y1,s32 border,s32 invert,s32 red,s32 green,s32 blue,s32 alpha) {
    u8 r=red,g=green,b=blue,a=alpha;
    s32 lightR,lightG,lightB,darkR,darkG,darkB;
    lightR=invert ? r>>1 : r<<1;
    lightG=invert ? g>>1 : g<<1;
    lightB=invert ? b>>1 : b<<1;
    darkR=invert ? r<<1 : r>>1;
    darkG=invert ? g<<1 : g>>1;
    darkB=invert ? b<<1 : b>>1;
    rectangle(x0,y0,x1,y1,r,g,b,a);
    rectangle(x0,y0,x1,y0+border,lightR,lightG,lightB,a);
    rectangle(x1-border,y0,x1,y1,lightR,lightG,lightB,a);
    rectangle(x0,y1-border,x1,y1,darkR,darkG,darkB,a);
    rectangle(x0,y0,x0+border,y1,darkR,darkG,darkB,a);
}

#include "globals/scene.h"
#include "protos/cache.h"
#include "protos/scene.h"

void Hud_DrawImageFrame(void *table, s32 frame, s32 x, s32 y, f32 scaleX, f32 scaleY, s32 scaled);

void Hud_DrawNumber(s32 value, f32 x, f32 y, f32 size, f32 arg4, s32 arg5, s32 rightAlign, s32 minDigits) {
    sptr sprites;
    s32 thousands;
    s32 hundreds;
    s32 tens;
    s32 ones;
    s32 rest;
    s32 last;
    s32 i;
    s32 place;
    s32 digit;
    f32 width;
    f32 scale;

    sprites = CScene__LoadGraphicSet(&gScene, 4, 0x10, 1);
    if (sprites == 0) {
        return;
    }
    if (value < 0) {
        value = -value;
        if (value < 0) {
            goto release;
        }
    }
    thousands = value * 0.001f;
    rest = value - thousands * 1000;
    hundreds = rest * 0.01f;
    rest -= hundreds * 100;
    tens = rest * 0.1f;
    ones = rest - tens * 10;
    last = 3;
    if (thousands == 0) {
        last = 2;
        if (hundreds == 0) {
            last = tens != 0;
        }
    }
    if (last < minDigits - 1) {
        last = minDigits - 1;
    }
    for (i = last; i >= 0; i--) {
        place = i;
        if (rightAlign) {
            place = last - i;
        }
        switch (place) {
        case 3:
            digit = thousands;
            break;
        case 2:
            digit = hundreds;
            break;
        case 1:
            digit = tens;
            break;
        case 0:
        default:
            digit = ones;
            break;
        }
        scale = 1.0f;
        if (digit == 1) {
            scale = 0.7f;
        }
        width = scale * (size * 13.0f);
        if (rightAlign) {
            x -= width;
        }
        if ((u32) digit < 10) {
            Hud_DrawImageFrame((void *) sprites, digit, (s16) x, (s16) y, size, arg4, arg5); /* The ROM's caller narrows the coordinates to s16 at the call. */
        }
        if (!rightAlign) {
            x += width;
        }
    }
release:
    CCache__Unlock(0, sprites);
}

#include "globals/menu.h"
#include "globals/onscreen.h"

sptr COnScreen__GetFontTexture(void);
extern void Render_SetCombineMode(s32 mode);
extern void Render_SetRenderMode(s32 mode);
void COnScreen__SetFontColor(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void COnScreen__SetFontScale(f32 arg0, f32 arg1);

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

/* Loads the texture COnScreen__GetFontTexture returns into TMEM, as gDPLoadTextureBlock
 * would: set the image, the load tile, sync, load the block, then the render
 * tile and its size (16x16). */
void COnScreen__SelectNormalFont(void) {
    Gfx *g;
    sptr tex = COnScreen__GetFontTexture();

    if (tex != 0) {
        gLoadedFont = 0;
        g = gDList++;
        g->w0 = 0xFD900000;
        g->w1 = tex;
        g = gDList++;
        g->w0 = 0xF5900100;
        g->w1 = 0x07010040;
        g = gDList++;
        g->w0 = 0xE6000000;
        g->w1 = 0;
        g = gDList++;
        g->w0 = 0xF3000000;
        g->w1 = 0x0703F800;
        g = gDList++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
        g = gDList++;
        g->w0 = 0xF5800300;
        g->w1 = 0x00010040;
        g = gDList++;
        g->w0 = 0xF2000000;
        g->w1 = 0x0003C03C;
    }
}

/* Loads a 16-bit I texture (gDPLoadTextureBlock-style) for the image returned
 * by COnScreen__GetFontTexture. */
void COnScreen__SelectFont3(void) {
    void *image = COnScreen__GetFontTexture();

    if (image != NULL) {
        gLoadedFont = 3;
        GFX_CMD(0xFD900000, GBI_ADDR(image));            /* gDPSetTextureImage */
        GFX_CMD(0xF5900100, 0x07010040);            /* gDPSetTile */
        GFX_CMD(0xE6000000, 0);                     /* gDPLoadSync */
        GFX_CMD(0xF3000000, 0x0703F800);            /* gDPLoadBlock */
        GFX_CMD(0xE7000000, 0);                     /* gDPPipeSync */
        GFX_CMD(0xF5800300, 0x00010040);            /* gDPSetTile */
        GFX_CMD(0xF2000000, 0x0003C03C);            /* gDPSetTileSize */
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A7AA4_de_3.c, func_802A8710_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void COnScreen__SelectSmallFont(void);

void COnScreen__SelectSmallFont(void) {
    Gfx *cmd;
    sptr v0;
    v0 = COnScreen__GetFontTexture();
    if (v0 != 0) {
        gLoadedFont = 1;
        cmd = gDList++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xFD) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = (GBI_ADDR(v0)); };
        cmd = gDList++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); };
        cmd = gDList++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xE6) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
        cmd = gDList++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xF3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(2048) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); };
        cmd = gDList++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
        cmd = gDList++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); };
        cmd = gDList++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xF2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(60) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(28) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); };
    }
}

void COnScreen__SelectTinyFont(void) {
    void *tex = COnScreen__GetFontTexture();
    if (tex != ((void *)0)) {
        gLoadedFont = 4;
        { Gfx *_g = gDList++; _g->w0 = (0xFD900000); _g->w1 = (GBI_ADDR(tex)); };
        { Gfx *_g = gDList++; _g->w0 = (0xF5900100); _g->w1 = (0x0700C040); };
        { Gfx *_g = gDList++; _g->w0 = (0xE6000000); _g->w1 = (0); };
        { Gfx *_g = gDList++; _g->w0 = (0xF3000000); _g->w1 = (0x0701F800); };
        { Gfx *_g = gDList++; _g->w0 = (0xE7000000); _g->w1 = (0); };
        { Gfx *_g = gDList++; _g->w0 = (0xF5800300); _g->w1 = (0x0000C040); };
        { Gfx *_g = gDList++; _g->w0 = (0xF2000000); _g->w1 = (0x0003C01C); };
    }
}

void Hud_DrawGlyph16x8(sptr texture, f32 x, f32 y, f32 sx, f32 sy) {
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
    yh = (y + sy * 8.0f) * 4.0f;
    xl = x * 4.0f;
    yl = y * 4.0f;
    g = gDList++;
    g->w0 = 0xFD900000;         /* gDPSetTextureImage(I, 16b, 1, texture) */
    g->w1 = texture;
    g = gDList++;
    g->w0 = 0xF5900000;         /* gDPSetTile(I, 16b, 0, 0, G_TX_LOADTILE, ...) */
    g->w1 = 0x0708C240;
    g = gDList++;
    g->w0 = 0xE6000000;         /* gDPLoadSync */
    g->w1 = 0;
    g = gDList++;
    g->w0 = 0xF3000000;         /* gDPLoadBlock(G_TX_LOADTILE, 0, 0, 31, 2048) */
    g->w1 = 0x0701F800;
    g = gDList++;
    g->w0 = 0xE7000000;         /* gDPPipeSync */
    g->w1 = 0;
    g = gDList++;
    g->w0 = 0xF5800200;         /* gDPSetTile(I, 4b, 1, 0, G_TX_RENDERTILE, ...) */
    g->w1 = 0x0008C240;
    g = gDList++;
    g->w0 = 0xF2000000;         /* gDPSetTileSize(G_TX_RENDERTILE, 0, 0, 60, 28) */
    g->w1 = 0x0003C01C;
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

extern f32 Menu_CharWidth(s32 ch, s32 next, f32 a, f32 b);    /* kerned advance */

/* The advance follows the currently loaded font. */
static __inline__ f32 fontGlyphWidth(void) {
    f32 result;

    switch (gLoadedFont) {
    case 5:
        result = 6.0f;
        break;
    case 4:
        result = 8.0f;
        break;
    case 1:
        result = 12.0f;
        break;
    case 0:
        result = 24.0f;
        break;
    case 3:
        result = 16.0f;
        break;
    default:
        result = 1.0f;
        break;
    }
    return result;
}

void COnScreen__DrawText(u8 *text, f32 x, f32 y, f32 width, f32 height, s32 shadow, s32 alpha, s32 language) {
    COnScreen *style = &gEngineApp_m_OnScreen;
    void *font;
    s32 special = 0;
    s32 resource;
    u8 ch;
    u8 next;
    u8 glyph;
    f32 advance;
    f32 sx;
    f32 sy;
    f32 a;
    f32 b;

    {
        Gfx *g;
        g = gDList++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
    }
    if (shadow) {
        Gfx *g;
        g = gDList++;
        g->w0 = 0xFB000000;     /* gDPSetEnvColor(0, 0, 0, alpha) */
        g->w1 = (u8) alpha;
        g = gDList++;
        g->w0 = 0xFA000000;     /* gDPSetPrimColor(0, 0, 0, 0, 0, 255) */
        g->w1 = 0xFF;
        x += style->m_ShadowXOff * (gScreenWidth / 284);
        y += style->m_ShadowYOff * (gScreenHeight / 222);
    } else {
        Gfx *g;
        g = gDList++;
        g->w0 = 0xFB000000;     /* gDPSetEnvColor */
        g->w1 = ((u8) style->m_TopR << 24) | ((u8) style->m_TopG << 16) | ((u8) style->m_TopB << 8) | (u8) alpha;
        g = gDList++;
        g->w0 = 0xFA000000;     /* gDPSetPrimColor(0, 0, ...) */
        g->w1 = ((u8) style->m_BottomR << 24) | ((u8) style->m_BottomG << 16) | ((u8) style->m_BottomB << 8) | 0xFF;
    }
    switch (language) {
    case 2:
    default:
        resource = 6;
        break;
    case 0:
        resource = 2;
        break;
    case 1:
        resource = 1;
        special = 1;
        break;
    }
    font = CScene__LoadGraphicSet(&gScene, resource, 0x10, 1);
    if (font) {
        while (text && (ch = *text) && ch != 10) {
            next = text[1];
            switch (language) {
            case 2:
            default:
                glyph = gEngineApp_m_OnScreen.m_CharacterSmallLookUpPos[ch];
                advance = fontGlyphWidth();
                break;
            case 1:
                glyph = gEngineApp_m_OnScreen.m_CharacterSmallLookUpPos[ch];
                advance = fontGlyphWidth();
                break;
            case 0:
                glyph = gEngineApp_m_OnScreen.m_CharacterSmallLookUpPos[0x80 + ch];
                a = fontGlyphWidth();
                b = fontGlyphWidth();
                advance = Menu_CharWidth(ch, next, a, b);
                break;
            }
            sx = width;
            sy = height;
            if (special && (glyph == 0x25 || glyph == 0x2B || glyph == 0x3D)) {
                sx = gScreenWidth * (1.0f / 284.0f);
                sy = gScreenHeight * (1.0f / 222.0f);
            }
            if (glyph != 255) {
                Hud_DrawImageFrame(font, glyph, (s16) x, (s16) y, sx, sy, 1);
            }
            x += advance * sx;
            text++;
        }
        CCache__Unlock(0, font);
    }
}

extern f32 Menu_TextWidth(u8 *text, f32 size, f32 spacing);   /* the string's width */

void COnScreen__DrawString(u8 *text, s32 x, s32 y, s32 alpha, s32 centred, s32 shadow, f32 scaleX, f32 scaleY) {
    COnScreen *style;
    f32 sizeFactor;
    f32 spacing;
    f32 size;
    f32 width;
    f32 limit;
    f32 ratio;
    f32 fx;
    f32 fy;
    f32 w;
    f32 h;
    s32 language;

    style = &gEngineApp_m_OnScreen;
    if (centred) {
        switch (gLoadedFont) {
        case 5:
            sizeFactor = 6.0f;
            break;
        case 4:
            sizeFactor = 8.0f;
            break;
        case 1:
            sizeFactor = 12.0f;
            break;
        case 0:
            sizeFactor = 24.0f;
            break;
        case 3:
            sizeFactor = 16.0f;
            break;
        default:
            sizeFactor = 1.0f;
            break;
        }
        size = style->m_FontXScale * scaleX * sizeFactor;
        switch (gLoadedFont) {
        case 5:
            spacing = 6.0f;
            break;
        case 4:
            spacing = 8.0f;
            break;
        case 1:
            spacing = 12.0f;
            break;
        case 0:
            spacing = 24.0f;
            break;
        case 3:
            spacing = 16.0f;
            break;
        default:
            spacing = 1.0f;
            break;
        }
        width = Menu_TextWidth(text, size, spacing);
        limit = gScreenWidth - 0x2C;
        if (limit < width) {
            ratio = limit / width;
            scaleX *= ratio;
            width *= ratio;
        }
        x -= (s32) (width * 0.5f);
    }
    {
        Gfx *g;
        g = gDList++;
        g->w0 = 0xE7000000;     /* gDPPipeSync */
        g->w1 = 0;
        g = gDList++;
        g->w0 = 0xFB000000;     /* gDPSetEnvColor */
        g->w1 = ((u8) style->m_TopR << 24) | ((u8) style->m_TopG << 16) | ((u8) style->m_TopB << 8) | (u8) alpha;
        g = gDList++;
        g->w0 = 0xFA000000;     /* gDPSetPrimColor(0, 0, ...) */
        g->w1 = ((u8) style->m_BottomR << 24) | ((u8) style->m_BottomG << 16) | ((u8) style->m_BottomB << 8) | 0xFF;
    }
    switch (gLoadedFont) {
    case 3:
        break;
    case 0:
    case 1:
    case 4:
        fx = x;
        w = style->m_FontXScale * scaleX;
        h = style->m_FontYScale * scaleY;
        language = gLoadedFont;
        fy = y;
        if (shadow) {
            COnScreen__DrawText(text, fx, fy, w, h, 1, alpha, language);
        }
        COnScreen__DrawText(text, fx, fy, w, h, 0, alpha, language);
        break;
    }
}

extern void Render_SetCombineMode(s32 arg0);
extern void Render_SetRenderMode(s32 arg0);

void Hud_SetupTextureMode(s32 color) {
    { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
    { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (20) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((((unsigned int)(0x1)) << 20))); };
    Render_SetCombineMode(0x1B);
    Render_SetRenderMode(0x19);
    {
        Gfx *cmd = gDList++;
        { Gfx *_gbi = (Gfx *)(cmd); _gbi->w0 = ((((unsigned int)(0xFB) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = ((((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(color) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); };
    }
    { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xD7) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (11)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (8)) | (((unsigned int)(((unsigned int)(1))) & (0xFFFFFFFFU >> (32 - (7)))) << (1))); _gbi->w1 = ((((unsigned int)(32768) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(32768) & (0xFFFFFFFFU >> (32 - (16)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (14) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((((unsigned int)(0x0)) << 14))); };
    { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (19) - (1)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((((unsigned int)(0x0)) << 19))); };
    if (gResolution == 0) { { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (12) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((((unsigned int)(0x0)) << 12))); }; } else { { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (12) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((((unsigned int)(0x2)) << 12))); }; }
}

void Hud_DrawMaskedBlock(void *pTexture, void *pOpacity, int XPos, int YPos, int TextureWidth, int TextureHeight) {
    { { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFD) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = (GBI_ADDR(pOpacity)); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE6) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)((((((TextureWidth*TextureHeight+3)>>2)-1)<2047?(((TextureWidth*TextureHeight+3)>>2)-1):2047)))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)((((1<<11)+(1>(TextureWidth/16)?1:(TextureWidth/16))-1)/(1>(TextureWidth/16)?1:(TextureWidth/16))))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((u32)(((((TextureWidth>>1)+7)>>3)))) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)(((TextureWidth-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)(((TextureHeight-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; }
    { { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFD) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = (GBI_ADDR(pTexture)); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE6) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)((((TextureWidth*TextureHeight-1)<2047?(TextureWidth*TextureHeight-1):2047)))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)((((1<<11)+(1>(TextureWidth*2/8)?1:(TextureWidth*2/8))-1)/(1>(TextureWidth*2/8)?1:(TextureWidth*2/8))))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((u32)((((TextureWidth*2)+7)>>3))) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)(((TextureWidth-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)(((TextureHeight-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; }
    { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((u32)((TextureWidth/16))) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(1) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)(((TextureWidth-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)(((TextureHeight-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); };
    { Gfx *g=gDList++; { Gfx *_gbi = (Gfx *)(g); _gbi->w0 = ((((unsigned int)(0xE4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)((XPos+TextureWidth)<<2) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((YPos+TextureHeight)<<2) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(XPos<<2) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(YPos<<2) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE1) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = ((unsigned int)(0)); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF1) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = ((unsigned int)((((u32)((1<<10)) & 0xFFFF) << ((16)))|(((u32)((1<<10)) & 0xFFFF) << ((0))))); }; }
}

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

void Hud_DrawMaskedBlockAt(u16 *image, u8 *mask, s32 x, s32 y, s32 width, s32 height, s32 s, s32 t) {
    gDPLoadMultiBlock_4b(gDList++, mask, 0x100, 0, G_IM_FMT_I, width, height, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTextureBlock(gDList++, image, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, height, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
    gDPSetTile(gDList++, G_IM_FMT_I, G_IM_SIZ_4b, width / 16, 0x100, 1, 0, 0, 0, 0, 0, 0, 0);
    gDPSetTileSize(gDList++, 1, 0, 0, (width - 1) << 2, (height - 1) << 2);
    gSPTextureRectangle(gDList++, x << 2, y << 2, (x + (width - s)) << 2, (y + (height - t)) << 2, 0, s << 5, t << 5, 1 << 10,
                        1 << 10);
}

void Hud_DrawMaskedBlockSized(u16 *image, u8 *mask, s32 x, s32 y, s32 texWidth, s32 texHeight, s32 width, s32 height) {
    gDPLoadMultiBlock_4b(gDList++, mask, 0x100, 0, G_IM_FMT_I, texWidth, texHeight, 0, G_TX_WRAP, G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTextureBlock(gDList++, image, G_IM_FMT_RGBA, G_IM_SIZ_16b, texWidth, texHeight, 0, G_TX_WRAP, G_TX_WRAP, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
    gDPSetTile(gDList++, G_IM_FMT_I, G_IM_SIZ_4b, texWidth / 16, 0x100, 1, 0, 0, 0, 0, 0, 0, 0);
    gDPSetTileSize(gDList++, 1, 0, 0, (texWidth - 1) << 2, (texHeight - 1) << 2);
    gSPTextureRectangle(gDList++, x << 2, y << 2, (x + width) << 2, (y + height) << 2, 0, 0, 0, 1 << 10, 1 << 10);
}

void Hud_DrawI4Block(void *pTexture, int XPos, int YPos, int TextureWidth, int TextureHeight) {
    { { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFD) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = (GBI_ADDR(pTexture)); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE6) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)((((((TextureWidth*TextureHeight+3)>>2)-1)<2047?(((TextureWidth*TextureHeight+3)>>2)-1):2047)))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)((((1<<11)+(1>(TextureWidth/16)?1:(TextureWidth/16))-1)/(1>(TextureWidth/16)?1:(TextureWidth/16))))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((u32)(((((TextureWidth>>1)+7)>>3)))) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)(((TextureWidth-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)(((TextureHeight-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; }
    { Gfx *g=gDList++; { Gfx *_gbi = (Gfx *)(g); _gbi->w0 = ((((unsigned int)(0xE4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)((XPos+TextureWidth)<<2) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((YPos+TextureHeight)<<2) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(XPos<<2) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(YPos<<2) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE1) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = ((unsigned int)(0)); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF1) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = ((unsigned int)((((u32)((1<<10)) & 0xFFFF) << ((16)))|(((u32)((1<<10)) & 0xFFFF) << ((0))))); }; }
}

/* One block of a tiled image: its size, then its 16-bit texels (and, for an
 * image with an opacity mask, the 4-bit mask after them). */
typedef struct {
    /* 0x0 */ u32 m_Width;
    /* 0x4 */ u32 m_Height;
    /* 0x8 */ u16 m_Pixels[1];
} TileBlock;
/* A tiled image: columns x rows blocks, row by row. */
typedef struct {
    /* 0x0 */ u16 m_nColumns;
    /* 0x2 */ u16 m_nRows;
    /* 0x4 */ u32 _pad;
    /* 0x8 */ TileBlock m_Blocks[1];
} TileGrid;
#include "structs/hud.h"

void Hud_GetImageFrame(FrameTable **ptable, s32 frame, void **pdata, u8 *pbyte);
void Hud_DrawI4BlockScaled(void *pTexture, f32 XPos, f32 YPos, int TextureWidth, int TextureHeight, f32 ScaleX, f32 ScaleY);

/* The next block: past the data of this one (`size` bytes), then its header. */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A7AA4_de_5.c, func_802A9EE8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
void Hud_DrawI4BlockScaled(void *pTexture, f32 XPos, f32 YPos, int TextureWidth, int TextureHeight, f32 ScaleX, f32 ScaleY) {
    int dsdx;
    int dtdy;
    int xl, yl, xh, yh;
    dsdx = 1 << 10;
    if (ScaleX != 0.0f) {
        dsdx = dsdx / ScaleX;
    }
    dtdy = 1 << 10;
    if (ScaleY != 0.0f) {
        dtdy = dtdy / ScaleY;
    }
    xl = XPos * 4.0f;
    xh = (XPos + TextureWidth * ScaleX) * 4.0f;
    yl = YPos * 4.0f;
    yh = (YPos + TextureHeight * ScaleY) * 4.0f;
    { { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFD) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((1) - 1) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = (GBI_ADDR(pTexture)); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE6) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)((((((TextureWidth*TextureHeight+3)>>2)-1)<2047?(((TextureWidth*TextureHeight+3)>>2)-1):2047)))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)((((1<<11)+(1>(TextureWidth/16)?1:(TextureWidth/16))-1)/(1>(TextureWidth/16)?1:(TextureWidth/16))))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (2)))) << (19)) | (((unsigned int)((u32)(((((TextureWidth>>1)+7)>>3)))) & (0xFFFFFFFFU >> (32 - (9)))) << (9)) | (((unsigned int)(256) & (0xFFFFFFFFU >> (32 - (9)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (14)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (10)) | (((unsigned int)((((unsigned int)(0x0)) << 1)) & (0xFFFFFFFFU >> (32 - (2)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (4)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (4)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)((u32)(((TextureWidth-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)((u32)(((TextureHeight-1)<<2))) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; }
    { Gfx *g=gDList++; { Gfx *_gbi = (Gfx *)(g); _gbi->w0 = ((((unsigned int)(0xE4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(xh) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(yh) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (3)))) << (24)) | (((unsigned int)(xl) & (0xFFFFFFFFU >> (32 - (12)))) << (12)) | (((unsigned int)(yl) & (0xFFFFFFFFU >> (32 - (12)))) << (0))); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE1) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = ((unsigned int)(0)); }; { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF1) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = ((unsigned int)((((u32)((dsdx)) & 0xFFFF) << ((16)))|(((u32)((dtdy)) & 0xFFFF) << ((0))))); }; }
}

#define NEXT_BLOCK(b, size)                                 \
    b = (TileBlock *) ((u8 *) (b) + (size));                \
    b = (TileBlock *) ((u8 *) (b) + 8)

static __inline__ void DrawScaled(TileGrid *grid, f32 x, f32 y, f32 scaleX, f32 scaleY) {
    TileBlock *block;
    u32 width, height;
    f32 fx, fy, col, row;

    fy = y;
    block = grid->m_Blocks;
    height = 0;
    row = grid->m_nRows;
    while (row-- != 0.0f) {
        col = grid->m_nColumns;
        fx = x;
        while (col-- != 0.0f) {
            height = block->m_Height;
            width = block->m_Width;
            Hud_DrawI4BlockScaled(block->m_Pixels, fx, fy, width, height, scaleX, scaleY);
            fx += (f32) width * scaleX;
            NEXT_BLOCK(block, (width * height) >> 1);
        }
        fy += (f32) height * scaleY;
    }
}

static __inline__ void drawTileBlock(TileGrid *grid, s16 x, s32 y) {
    TileBlock *block;
    u32 width, height;
    s32 ix, iy;
    s32 i, j;

    iy = y;
    block = grid->m_Blocks;
    height = 0;
    j = grid->m_nRows;
    while (--j != -1) {
        i = grid->m_nColumns;
        ix = x;
        while (--i != -1) {
            width = block->m_Width;
            height = block->m_Height;
            Hud_DrawI4Block(block->m_Pixels, ix, (s16) iy, width, height);
            ix += width;
            NEXT_BLOCK(block, (width * height) >> 1);
        }
        iy += height;
    }
}

static __inline__ void DrawScaledMasked(TileGrid *grid, f32 x, f32 y, f32 scaleX, f32 scaleY) {
    TileBlock *block;
    u32 width, height, size;
    f32 fx, fy, col, row;

    fy = y;
    block = grid->m_Blocks;
    height = 0;
    row = grid->m_nRows;
    while (row-- != 0.0f) {
        col = grid->m_nColumns;
        fx = x;
        while (col-- != 0.0f) {
            height = block->m_Height;
            width = block->m_Width;
            size = width * height;
            Hud_DrawMaskedBlockScaled(block->m_Pixels, block->m_Pixels + size, fx, fy, width, height, scaleX, scaleY);
            fx += (f32) width * scaleX;
            NEXT_BLOCK(block, size * 2 + (size >> 1));
        }
        fy += (f32) height * scaleY;
    }
}

static __inline__ void DrawMasked(TileGrid *grid, s16 x, s32 y) {
    TileBlock *block;
    u32 width, height, size;
    s32 ix, iy;
    s32 i, j;

    iy = y;
    block = grid->m_Blocks;
    j = grid->m_nRows;
    height = 0;
    while (--j != -1) {
        i = grid->m_nColumns;
        ix = x;
        while (--i != -1) {
            height = block->m_Height;
            width = block->m_Width;
            size = width * height;
            Hud_DrawMaskedBlock(block->m_Pixels, block->m_Pixels + size, ix, (s16) iy, width, height);
            ix += width;
            NEXT_BLOCK(block, size * 2 + (size >> 1));
        }
        iy += height;
    }
}

/* Draws frame `frame` of a tiled image at (x, y): block by block, row by row,
 * scaled by (scaleX, scaleY) when `scaled` is set; an image whose frame byte
 * is clear carries an opacity mask after each block's texels. */
void Hud_DrawImageFrame(void *table, s32 frame, s32 x, s32 y, f32 scaleX, f32 scaleY, s32 scaled) {
    TileGrid *grid;
    u8 opaque;

    {
        Gfx *g = gDList++;
        g->w0 = 0xE7000000;                         /* gDPPipeSync */
        g->w1 = 0;
    }
    if (table != NULL) {
        Hud_GetImageFrame((FrameTable **) table, frame, (void **) &grid, &opaque);
        if (grid != NULL) {
            if (opaque) {
                if (scaled) {
                    DrawScaled(grid, (s16) x, (s16) y, scaleX, scaleY);
                } else {
                    drawTileBlock(grid, x, y);
                }
            } else if (scaled) {
                DrawScaledMasked(grid, (s16) x, (s16) y, scaleX, scaleY);
            } else {
                DrawMasked(grid, x, y);
            }
            CCache__LockForDisplayList(0, table);
        }
    }
}

#include "structs/player.h"
/* Off screen, no script, value -1, at (arg1, arg2). (The N64 view this replaced put the position
 * in m_Mode/m_Time on the host, its m_pScript being pointer-wide: no HUD drew.) */
void COverlay__Construct(COverlay *arg0, s16 arg1, s16 arg2) {
    arg0->m_Value = -1;
    arg0->m_PositionX = arg1;
    arg0->m_bOnScreen = 0;
    arg0->m_Mode = 0;
    arg0->m_PositionY = arg2;
}

void COverlay__StartScript(COverlay *arg0, s32 *arg1) {
    arg0->m_pScript = arg1;
    arg0->m_Mode = 2;
    COverlay__Update(arg0);
}

#include "globals/hud.h"

void COnScreen__SetBoss(COnScreen *arg0, CGameObjectInstance *arg1, char *arg2) {
    arg0->m_pBoss = arg1;
    arg0->m_pBossName = arg2;
    arg0->m_BossPercent = 100;
    COverlay__StartScript(&arg0->m_BossOverlay, sBossBarOverlayScript);
}

typedef struct {
    /* 0x00 */ u8 pad0[0x48];
    /* 0x48 */ u8 sub48[0x84 - 0x48];
    /* 0x84 */ s32 unk84;
    /* 0x88 */ s32 unk88;
    /* 0x8C */ s32 unk8C;
} Unk0027AD54;

/* Host body: the view above writes the second boss bar at N64 offsets (0x48 the overlay, 0x84 the
 * name, 0x88 the boss, 0x8C the percent); the host's COnScreen has a pointer-wide
 * COverlay.m_pScript, so they land in the wrong members (round 17 f4). The same stores by member. */
void COnScreen__SetBoss2(void *arg0, sptr arg1, sptr arg2) {
    COnScreen *os = (COnScreen *)arg0;

    os->m_pBoss2 = (struct CGameObjectInstance *)arg1;
    os->m_pBossName2 = (char *)arg2;
    os->m_BossPercent2 = 0x64;
    COverlay__StartScript(&os->m_BossOverlay2, sBossBar2OverlayScript);
}

/* Host body: the view stores the percent at m_BossPercent's N64 offset 0x44, which on the host is
 * COverlay padding: the bar never went down (round 17 f4). The same store by member. */
void COnScreen__SetBossPercent(void *arg0, s32 arg1) {
    ((COnScreen *)arg0)->m_BossPercent = arg1;
}

void COnScreen__SetBossPercent2(COnScreen *arg0, s32 arg1) {
    arg0->m_BossPercent2 = arg1;
}

/* Host body: the view stores m_pBoss's 0 at its N64 offset 0x40, which on the host (a
 * pointer-wide COverlay.m_pScript) is m_BossOverlay.m_Value: the boss was never cleared and its
 * bar stayed up after the fight (round 17 f4). The same store by member. */
void COnScreen__ClearBoss(void *arg0) {
    ((COnScreen *)arg0)->m_pBoss = NULL;
}

void COnScreen__ClearBoss2(COnScreen *arg0) {
    arg0->m_pBoss2 = 0;
}

#ifdef TARGET_PORT
#include "structs/player.h"

/* Host body: the ROM tests m_pBoss and m_pBoss2 at their N64 offsets (0x40, 0x88). On the host
 * COverlay holds a pointer-wide m_pScript, so 0x40 is m_BossOverlay.m_Value (0 until the bar is
 * drawn: never) and 0x88 lands inside m_BossOverlay2: no boss bar was ever drawn (round 17 f4,
 * the Primagen's green bar, seen against the N64 from the same warp). The same tests by member. */
void COnScreen__DrawBossBars(void *arg0) {
    COnScreen *os = (COnScreen *)arg0;

    Hud_SetupTextureMode(gEngineApp_m_Settings.hudAlpha);
    if (os->m_pBoss != NULL) {
        COnScreen__DrawBossBar(arg0, 1);
    }
    if (os->m_pBoss2 != NULL) {
        COnScreen__DrawBossBar(arg0, 2);
    }
}
#else
void COnScreen__DrawBossBars(void *arg0) {
    u8 pad[0x100];

    Hud_SetupTextureMode(gEngineApp_m_Settings.hudAlpha);
    if (*(s32 *)((u8 *)arg0 + 0x40) != 0) {
        COnScreen__DrawBossBar(arg0, 1);
    }
    if (*(s32 *)((u8 *)arg0 + 0x88) != 0) {
        COnScreen__DrawBossBar(arg0, 2);
    }
}
#endif
#include "globals/engine.h"

sptr Hud_LoadGraphicSet(s32 arg0) {
    sptr r;

    if (gPersistentDataLoaded != 0) {
        r = CScene__LoadGraphicSet(&gScene, arg0, 3, 1);
    } else {
        r = 0;
    }
    return r;
}

/* Frame `frame` (wrapped to the count) of *ptable: its data in *pdata and its
 * byte in *pbyte; *pdata is NULL with no table. */
void Hud_GetImageFrame(FrameTable **ptable, s32 frame, void **pdata, u8 *pbyte) {
    FrameTable *table;
    FrameEntry *entry;
    s32 offset;
    u8 byte;
    s32 i;

    *pdata = NULL;
    if (ptable != NULL) {
        table = *ptable;
        entry = table->m_Frames;
        i = frame % table->m_nFrames;
        offset = entry[i].m_Offset;
        byte = entry[i].m_Byte;
        *pbyte = byte;
        *pdata = (u8 *)table + offset;
    }
}

void Hud_GetSpriteSize(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3)
{
  void *sp10;
  s32 sp14;
  void *new_var;
  sptr temp_v0;
  if (gPersistentDataLoaded != 0)
  {
    temp_v0 = CScene__LoadGraphicSet((&gScene), arg0, 0, 1);
    if (temp_v0 != 0)
    {
      Hud_GetImageFrame((FrameTable **) temp_v0, arg1, &sp10, (u8 *) (&sp14));
      new_var = sp10;
      *arg2 = (s32) (*((u16 *) (((s8 *) new_var) + 4)));
      *arg3 = (s32) (*((u16 *) (((s8 *) new_var) + 6)));
      CCache__Unlock(0, temp_v0);
    }
  }
}

void Render_SetRenderMode(s32);
void Render_SetCombineMode(s32);

void Hud_SetupFillMode(void) {
    Gfx *g;

    g = gDList++;
    g->w0 = 0xE7000000;
    g->w1 = 0;
    g = gDList++;
    g->w0 = 0xE3000A01;
    g->w1 = 0;
    Render_SetRenderMode(0x15);
    Render_SetCombineMode(0x19);
}

void Hud_FillRect(s32 x0,s32 y0,s32 x1,s32 y1,u8 r,u8 g,u8 b,u8 a) {
 { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
 { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xFA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(255) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((((unsigned int)(r) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(g) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(b) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(a) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); };
 { Gfx *_gbi = (Gfx *)(gDList++); _gbi->w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((x1))>((0))?((x1)):((0))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((y1))>((0))?((y1)):((0))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->w1 = ((((unsigned int)(((x0))>((0))?((x0)):((0))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((y0))>((0))?((y0)):((0))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
}

/* The font colour COnScreen__SetFontColor sets (gEngineApp_m_OnScreen_m_TopR): six words, in argument order. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
} FontColor;
/* m_TopR and the five words after it, as one view (the field's real type changes the ROM's code here). */
#define sFontColor (*(FontColor *) &gEngineApp_m_OnScreen_m_TopR)

void COnScreen__SetFontColor(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    sFontColor.unk0 = arg0;
    sFontColor.unk4 = arg1;
    sFontColor.unk8 = arg2;
    sFontColor.unkC = arg3;
    sFontColor.unk10 = arg4;
    sFontColor.unk14 = arg5;
}

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
} FontScale;
/* m_FontXScale and the word after it, as one view (the field's real type changes the ROM's code here). */
#define sFontScale (*(FontScale *) &gEngineApp_m_OnScreen_m_FontXScale)

void COnScreen__SetFontScale(f32 arg0, f32 arg1) {
    sFontScale.x = arg0;
    sFontScale.y = arg1;
}

void COnScreen__SelectFont2(void) {
    COnScreen__SelectSmallFont();
    gLoadedFont = 2;
}

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

void COnScreen__DrawTextShadowed(sptr arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    if (arg5 != 0) {
        COnScreen__DrawText(arg0, arg1, arg2, arg3, arg4, 1, arg6, arg7);
    }
    COnScreen__DrawText(arg0, arg1, arg2, arg3, arg4, 0, arg6, arg7);
}

#include "structs/frame.h"

s32 Hud_DrawSprite(s32 arg0, s32 arg1, s16 arg2, s16 arg3, f32 arg4, f32 arg5, s32 arg6) {
    sptr temp_v0;

    if (((u32) (gDisplayListSize - ((u32) ((sptr) gDList - (sptr) gEngineApp_m_pCurrentFrameData->m_pDisplayList) / GFX_SIZE)) < 0x3E8U) || (gPersistentDataLoaded == 0) || (temp_v0 = CScene__LoadGraphicSet(&gScene, arg0, 0, 1), (temp_v0 == 0))) {
        return 0;
    }
    Hud_DrawImageFrame((void *) temp_v0, arg1, (s16) arg2, (s16) arg3, arg4, arg5, arg6); /* The ROM's caller narrows the coordinates to s16 at the call. */
    CCache__Unlock(0, temp_v0);
    return 1;
}

typedef struct {
    /* 0x00 */ s32 offset;
    /* 0x04 */ u8 flag;
    /* 0x05 */ u8 pad5[3];
} ChunkEntry;
typedef struct {
    /* 0x00 */ s16 count;
    /* 0x02 */ u8 pad2[6];
    /* 0x08 */ ChunkEntry entries[1];
} ChunkTable;
#include "structs/cache.h"
typedef struct {
    /* 0x00 */ u8 *data;
    /* 0x04 */ u8 flag;
} FontChunk;
void *func_0029E460(void *dst, const void *src, s32 size); /* memcpy */

/* Loads the 0x90-byte block of chunk 42 of resource 2 into sOnScreenFontTextureBuffer and
 * points sOnScreenFontTexture at it (NULL until it is loaded). */
void COnScreen__LoadFontTexture(void) {
    CMemEntry *res;
    FontChunk chunk;
    ChunkTable *table;
    ChunkEntry *entries;
    u8 *data;
    s32 index;

    sOnScreenFontTexture = NULL;
    res = CScene__LoadGraphicSet(&gScene, 2, 0, 1);
    if (res != NULL) {
        table = res->data;
        index = 42 % table->count;
        chunk.data = NULL;
        entries = table->entries;
        data = (u8 *) table + entries[index].offset;
        chunk.flag = entries[index].flag;
        chunk.data = data;
        if (chunk.data != NULL) {
            chunk.data += 0x10;
        }
        func_0029E460(sOnScreenFontTextureBuffer, chunk.data, 0x90);
        CCache__Unlock(0, res);
        sOnScreenFontTexture = sOnScreenFontTextureBuffer;
    }
}

sptr COnScreen__GetFontTexture(void) {
    return (sptr) sOnScreenFontTexture;
}
void Hud_DrawMaskedImage(u16 *grid, s16 x0, s32 y);

void Hud_DrawMaskedImage(u16 *grid, s16 x0, s32 y) {
    s32 *tile;
    s32 rows;
    s32 cols;
    s32 x;
    s32 w;
    s32 h;
    u32 area;
    tile = (s32 *)(grid + 4);
    h = 0;
    rows = grid[1];
    while (--rows != -1) {
        x = x0;
        cols = grid[0];
        while (--cols != -1) {
            w = tile[0];
            h = tile[1];
            area = w * h;
            Hud_DrawMaskedBlock(tile + 2, (char *)tile + (area * 2 + 8), x, (s16) y, w, h); /* y narrowed to s16 at the call, as in the ROM */
            x += w;
            tile = (s32 *)((char *)tile + (area * 2 + (area >> 1)));
            tile += 2;
        }
        y += h;
    }
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002ABED4.c, func_002ABED4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void Hud_DrawMaskedImageScaled(u16 *hdr, f32 x0, f32 y, f32 sx, f32 sy) {
    f32 rows, cols, x;
    u32 w, h;
    u8 *p;
    p = (u8 *)hdr + 8;
    h = 0;
    rows = hdr[1];
    while (rows-- != 0.0f) {
        cols = hdr[0];
        x = x0;
        while (cols-- != 0.0f) {
            h = ((AtlasCell *)p)->h;
            w = ((AtlasCell *)p)->w;
            Hud_DrawMaskedBlockScaled(p + 8, p + (w * h * 2 + 8), x, y, w, h, sx, sy);
            x += (f32)w * sx;
            p += (w * h * 2) + ((w * h) >> 1);
            p += 8;
        }
        y += (f32)h * sy;
    }
}
void Hud_DrawI4Image(u16 *grid, s16 x0, s32 y);

void Hud_DrawI4Image(u16 *grid, s16 x0, s32 y) {
    s32 *tile;
    s32 rows;
    s32 cols;
    s32 x;
    s32 w;
    s32 h;
    tile = (s32 *)(grid + 4);
    h = 0;
    rows = grid[1];
    while (--rows != -1) {
        x = x0;
        cols = grid[0];
        while (--cols != -1) {
            w = tile[0];
            h = tile[1];
            Hud_DrawI4Block(tile + 2, x, (s16) y, w, h); /* y narrowed to s16 at the call, as in the ROM */
            x += w;
            tile = (s32 *)((char *)tile + ((u32)(w * h) >> 1));
            tile += 2;
        }
        y += h;
    }
}
void Hud_DrawI4ImageScaled(u16 *grid, f32 x0, f32 y, f32 sx, f32 sy);

void Hud_DrawI4ImageScaled(u16 *grid, f32 x0, f32 y, f32 sx, f32 sy) {
    u32 *tile;
    f32 rows;
    f32 cols;
    f32 x;
    u32 w;
    u32 h;
    tile = (u32 *)(grid + 4);
    h = 0;
    rows = grid[1];
    while (rows-- != 0.0f) {
        x = x0;
        cols = grid[0];
        while (cols-- != 0.0f) {
            w = tile[0];
            h = tile[1];
            Hud_DrawI4BlockScaled(tile + 2, x, y, w, h, sx, sy);
            x += w * sx;
            tile = (u32 *)((char *)tile + ((w * h) >> 1));
            tile += 2;
        }
        y += h * sy;
    }
}

/* Builds a 128-entry lookup table: every entry 0xFF (none), then each
 * (index, value) pair of the list, which ends at a negative index. The
 * counter is the first argument, which callers pass for nothing. */
void COnScreen__BuildCharacterLookUp(u8 i, s8 *pairs, u8 *table) {
    s8 *p;
    u8 slot;
    s8 index;

    i = 0;
    do {
        table[i] = 0xFF;
        i++;
    } while ((s8)i >= 0);
    i = 0;
    do {
        p = pairs + i * 2;
        slot = *p;
        index = *p;
        if (index >= 0) {
            table[slot] = p[1];
        }
        i++;
    } while (index >= 0);
}

f32 COnScreen__GetFontAdvance(void) {
    switch (gLoadedFont) {
        case 5:
            return 6.0f;
        case 4:
            return 8.0f;
        case 1:
            return 12.0f;
        case 0:
            return 24.0f;
        case 3:
            return 16.0f;
        default:
            return 1.0f;
    }
}

void func_0027BB74(void) {

}
