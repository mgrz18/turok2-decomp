#include "common.h"

void RumblePool_FreeAll(s32);
s32 Controller_ResetAllRumble(void);

extern s32 D_800F5A50;
extern s32 D_800F5F8C;
extern s32 D_80130910;

void PauseMenu_Close_2(void) {
    s32 temp_v0;
    s32 var_a0;
    s32 var_v1;

    Controller_ResetAllRumble();
    var_v1 = 0;
    var_a0 = 0;
    do {
        temp_v0 = *(((__typeof__(&D_80130910))((s8 *)&D_80130910 + ((var_v1 << 2)))));
        var_v1 += 1;
        *(((__typeof__(&D_800F5F8C))((s8 *)&D_800F5F8C + var_a0))) = temp_v0;
        var_a0 += 0x224;
    } while (var_v1 < 4);
    RumblePool_FreeAll((s32) &D_800F5A50);
}
