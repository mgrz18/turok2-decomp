#include "common.h"
#include "structs/controller.h"
#include "globals/rumble.h"
#include "globals/menu.h"

void Controller_ResetAllRumble(void);
extern void RumblePool_FreeAll(RumblePool *pool);

/* Restores the per-controller words saved by PauseMenu_Init_2. */
void PauseMenu_Close_2(void) {
    s32 i;

    Controller_ResetAllRumble();
    for (i = 0; i < 4; i++) {
        gControllers[i].rumbleOn = sPauseRumbleOn[i];
    }
    RumblePool_FreeAll(&gRumblePool);
}
