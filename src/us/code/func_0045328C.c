#include "common.h"
#include "structs/controller.h"
#include "globals/menu.h"

void RumblePool_FreeAll(s32);
s32 Controller_ResetAllRumble(void);

extern s32 gRumblePool;

/* Restores the per-controller words saved by PauseMenu_Init_2. */
void PauseMenu_Close_2(void) {
    s32 i;

    Controller_ResetAllRumble();
    i = 0;
    do {
        gControllers[i].rumbleOn = sPauseRumbleOn[i];
        i++;
    } while (i < 4);
    RumblePool_FreeAll((s32) &gRumblePool);
}
