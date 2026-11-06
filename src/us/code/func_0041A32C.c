#include "common.h"
#include "structs/controller.h"
#include "globals/menu.h"


extern u8 gRumblePool[];
extern void Controller_ResetAllRumble(void);
extern void RumblePool_FreeAll(void *);

/* Restores the per-controller words saved by PauseMenu_Init. */
void PauseMenu_Close(void) {
    s32 i;

    Controller_ResetAllRumble();
    for (i = 0; i < 4; i++) {
        gControllers[i].rumbleOn = sPauseRumbleOn[i];
    }
    RumblePool_FreeAll(gRumblePool);
}
