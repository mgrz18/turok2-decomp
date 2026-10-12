#include "common.h"

typedef struct { u32 w0; u32 w1; } Gfx;

extern Gfx *D_800D8E24;
void func_00207FEC(s32);
void func_00207A70(s32);

void func_0027AF4C(void) {
    Gfx *g;

    g = D_800D8E24++;
    g->w0 = 0xE7000000;
    g->w1 = 0;
    g = D_800D8E24++;
    g->w0 = 0xE3000A01;
    g->w1 = 0;
    func_00207FEC(0x15);
    func_00207A70(0x19);
}
