#include "common.h"
#include "globals/render.h"
#include "globals/engine.h"

extern s32 Render_DrawTextLines(sptr, s32, void *, s32, s32, s32, f32, f32);

void EngineMode01_Legal_Draw(sptr arg0) {
    f32 x;

    if (gScreenWidth >= 0x11D) {
        x = gScreenWidth / 2 - 12.0f;
    } else {
        x = gScreenWidth / 2;
    }
    Render_DrawTextLines(arg0, 1, sLegalText, x, 12, 10, 0.7f, 1.0f);
}
