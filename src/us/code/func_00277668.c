/* Draw the outline of a box, `border` pixels thick, in one flat colour: the top, bottom, left and right bars as fill rectangles. */
/* Same display-list style as its neighbour Hud_DrawBevelBox (after Turok: Rage Wars' rectangle()). */

#include "common.h"
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
