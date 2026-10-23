#include "common.h"
#include "gfx.h"

extern void func_00279028(s32);
extern void func_00254D5C(void *, s32, s32);
extern void func_0024B3E4(void *, s32, s32);

typedef struct {
    /* 0x000 */ u8 pad0[0x380];
    /* 0x380 */ s32 field380;
} Unk254CC0;

void func_00254CC0(Unk254CC0 *arg0, s32 arg1, s32 arg2) {
    Gfx *g;

    func_00279028(0xFF);
    g = D_800D8E24++;
    g->w0 = 0xE3001201; /* gDPSetTextureFilter(G_TF_BILERP) */
    g->w1 = 0x2000;
    if (arg0->field380) {
        func_00254D5C(arg0, arg1, arg2);
    } else {
        func_0024B3E4(arg0, arg1, arg2);
    }
}
