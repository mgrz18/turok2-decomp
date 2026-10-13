#include "common.h"
#include "gfx.h"

void func_0020B5D4(void);
void func_0023118C(void *, s32);
void func_0020B624(void);

void func_002240C4(u8 *arg0, s32 arg1) {
    Gfx *g;

    /* gSPClipRatio(FRUSTRATIO_1) */
    g = D_800D8E24++;
    g->w0 = 0xDB040004;
    g->w1 = 1;
    g = D_800D8E24++;
    g->w0 = 0xDB04000C;
    g->w1 = 1;
    g = D_800D8E24++;
    g->w0 = 0xDB040014;
    g->w1 = 0xFFFF;
    g = D_800D8E24++;
    g->w0 = 0xDB04001C;
    g->w1 = 0xFFFF;
    func_0020B5D4();
    func_0023118C(arg0 + 0x18B0, arg1);
    func_0020B624();
}
