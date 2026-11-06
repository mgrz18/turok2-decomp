#include "common.h"
#include "gfx.h"


extern Gfx *D_800D8E24;
void Render_SetRenderMode(s32);
void Render_SetCombineMode(s32);

void Hud_SetupFillMode(void) {
    Gfx *g;

    g = D_800D8E24++;
    g->w0 = 0xE7000000;
    g->w1 = 0;
    g = D_800D8E24++;
    g->w0 = 0xE3000A01;
    g->w1 = 0;
    Render_SetRenderMode(0x15);
    Render_SetCombineMode(0x19);
}
