#include "common.h"
#include "structs/demo.h"

s32 func_0042EF2C(s32);

extern s32 D_004384E8;

void func_0042F590(void) {
    DemoPlayer *state = &D_800F56B0;
    s32 temp_v0;

    if (state->active != 0) {
        func_0042EF2C(*(((__typeof__(&D_004384E8))((s8 *)&D_004384E8 + (state->index * 4)))));
        temp_v0 = state->index + 1;
        state->index = temp_v0;
        if (*(((__typeof__(&D_004384E8))((s8 *)&D_004384E8 + (temp_v0 * 4)))) == 0) {
            state->index = 0;
        }
    }
}
