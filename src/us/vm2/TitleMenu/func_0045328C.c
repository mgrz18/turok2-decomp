#include "common.h"
#include "structs/controller.h"
#include "globals/menu.h"
#include "globals/rumble.h"

void RumblePool_FreeAll(RumblePool *pool);
s32 Controller_ResetAllRumble(void);


/* Restores the per-controller words saved by PauseMenu_Init_2. */
void PauseMenu_Close_2(void) {
    s32 i;

    Controller_ResetAllRumble();
    i = 0;
    do {
        gControllers[i].rumbleOn = sPauseRumbleOn[i];
        i++;
    } while (i < 4);
    RumblePool_FreeAll(&gRumblePool);
}
