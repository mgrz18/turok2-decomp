#include "common.h"
#include "structs/controller.h"


extern s32 D_80130910[4];   /* saved per-controller words */
extern u8 D_800F5A50[];
extern void Controller_ResetAllRumble(void);
extern void RumblePool_FreeAll(void *);

/* Restores the per-controller words saved by PauseMenu_Init. */
void PauseMenu_Close(void) {
    s32 i;

    Controller_ResetAllRumble();
    for (i = 0; i < 4; i++) {
        D_800F5EC0[i].rumbleOn = D_80130910[i];
    }
    RumblePool_FreeAll(D_800F5A50);
}
