#include "common.h"

void RumblePool_FreeAll(s32);
s32 Controller_ResetAllRumble(void);

extern s32 gRumblePool;
extern s32 gControllers_rumbleOn;
extern s32 sPauseRumbleOn;

void PauseMenu_Close_2(void) {
    s32 temp_v0;
    s32 var_a0;
    s32 var_v1;

    Controller_ResetAllRumble();
    var_v1 = 0;
    var_a0 = 0;
    do {
        temp_v0 = *(((__typeof__(&sPauseRumbleOn))((s8 *)&sPauseRumbleOn + ((var_v1 << 2)))));
        var_v1 += 1;
        *(((__typeof__(&gControllers_rumbleOn))((s8 *)&gControllers_rumbleOn + var_a0))) = temp_v0;
        var_a0 += 0x224;
    } while (var_v1 < 4);
    RumblePool_FreeAll((s32) &gRumblePool);
}
