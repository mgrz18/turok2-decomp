/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/GFX/TEXT/802A8A94.CPP, func_802A822C_de, with Turok 2's layouts. */
/* Draw a number of up to four digits with the font sprites: at least minDigits digits, 13 units apart per
 * digit (0.7 of that for a one) scaled by the size, laid out right to left when rightAlign is set.
 * Unlike Rage Wars, Turok 2 draws no minus sign: a negative value is drawn as its magnitude. */

#include "common.h"
#include "globals/scene.h"
#include "protos/cache.h"


extern s32 CScene__LoadGraphicSet(void *set, s32 a, s32 b, s32 c);  /* Render_StartWorldEffect */
extern void Hud_DrawImageFrame(s32 sprites, s32 digit, s16 x, s16 y, f32 size, f32 arg4, s32 arg5);

void Hud_DrawNumber(s32 value, f32 x, f32 y, f32 size, f32 arg4, s32 arg5, s32 rightAlign, s32 minDigits) {
    s32 sprites;
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
            Hud_DrawImageFrame(sprites, digit, x, y, size, arg4, arg5);
        }
        if (!rightAlign) {
            x += width;
        }
    }
release:
    CCache__Unlock(0, sprites);
}
