#include "common.h"
#include "gfx.h"

extern void Hud_SetupTextureMode(s32);
extern void CBarrel__DrawEight(void *, s32, s32);
extern void CBarrel__DrawRing(void *, s32, s32);

typedef struct {
    /* 0x000 */ u8 pad0[0x380];
    /* 0x380 */ s32 field380;
} Unk254CC0;

void CBarrel__Draw(Unk254CC0 *arg0, s32 arg1, s32 arg2) {
    Gfx *g;

    Hud_SetupTextureMode(0xFF);
    g = gDList++;
    g->w0 = 0xE3001201; /* gDPSetTextureFilter(G_TF_BILERP) */
    g->w1 = 0x2000;
    if (arg0->field380) {
        CBarrel__DrawEight(arg0, arg1, arg2);
    } else {
        CBarrel__DrawRing(arg0, arg1, arg2);
    }
}
