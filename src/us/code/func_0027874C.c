/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/GFX/TEXT/802A8A94_2.CPP, func_802A88F0_de, with Turok 2's layouts. */
/* Draw one line of text (up to '\n') in the current font style gEngineApp_m_OnScreen: set the environment and
 * primitive colours (black, shifted by the style's shadow offset scaled to the screen, for the shadow
 * pass), load the font for the language, then draw each glyph through the language's lookup table,
 * advancing by the glyph width of the loaded font gLoadedFont (kerned for language 0). In language 1
 * the glyphs '%', '+' and '=' are drawn at screen scale. */

#include "common.h"
#include "gfx.h"
#include "globals/onscreen.h"
#include "globals/scene.h"
#include "globals/render.h"
#include "globals/menu.h"
#include "protos/cache.h"
#include "protos/scene.h"

typedef struct FontStyle {
#ifdef TARGET_PORT /* COnScreen's colours, scale and spacing at their host offset (pointers come before them) */
    u8 pad0[T2_OFFSETOF(COnScreen, m_TopR)];
#else
    /* 0x00 */ u8 pad0[0xA0];
#endif
    /* 0xA0 */ s32 envR;
    /* 0xA4 */ s32 envG;
    /* 0xA8 */ s32 envB;
    /* 0xAC */ s32 primR;
    /* 0xB0 */ s32 primG;
    /* 0xB4 */ s32 primB;
    /* 0xB8 */ f32 width;
    /* 0xBC */ f32 height;
    /* 0xC0 */ s32 shadowX;
    /* 0xC4 */ s32 shadowY;
} FontStyle;

extern u8 D_80110238[];         /* glyph lookup, language 0 */

extern f32 Menu_CharWidth(s32 ch, s32 next, f32 a, f32 b);    /* kerned advance */
extern void Hud_DrawImageFrame(void *font, s32 glyph, s16 x, s16 y, f32 sx, f32 sy, s32 flag);

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
    FontStyle *style = &gEngineApp_m_OnScreen;
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
        x += style->shadowX * (gScreenWidth / 284);
        y += style->shadowY * (gScreenHeight / 222);
    } else {
        Gfx *g;
        g = gDList++;
        g->w0 = 0xFB000000;     /* gDPSetEnvColor */
        g->w1 = ((u8) style->envR << 24) | ((u8) style->envG << 16) | ((u8) style->envB << 8) | (u8) alpha;
        g = gDList++;
        g->w0 = 0xFA000000;     /* gDPSetPrimColor(0, 0, ...) */
        g->w1 = ((u8) style->primR << 24) | ((u8) style->primG << 16) | ((u8) style->primB << 8) | 0xFF;
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
                glyph = D_80110238[ch];
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
