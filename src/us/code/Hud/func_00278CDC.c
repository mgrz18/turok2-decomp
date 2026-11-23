/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/GFX/TEXT/802A8A94_3.CPP, func_802A8F28_de, with Turok 2's layouts. */
/* Draw a string in the current font style gEngineApp_m_OnScreen: when centring, measure it at the style size scaled by
 * the factor for the loaded font gLoadedFont, shrink it to fit 44 pixels inside the screen width and shift x
 * left by half the width; set the environment colour (with the given alpha) and the primitive colour from
 * the style, then draw the string (first its shadow when asked) for the fonts that draw this way. */

#include "common.h"
#include "gfx.h"
#include "globals/onscreen.h"
#include "globals/render.h"
#include "globals/menu.h"
#include "structs/player.h"



extern f32 Menu_TextWidth(u8 *text, f32 size, f32 spacing);   /* the string's width */
extern void COnScreen__DrawText(u8 *text, f32 x, f32 y, f32 width, f32 height, s32 shadow, s32 alpha, s32 language);

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
