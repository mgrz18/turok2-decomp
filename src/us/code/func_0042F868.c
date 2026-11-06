#include "common.h"
#include "structs/demo.h"

extern s32 D_800B6D20;

/* Folds the timers' values into the state's checksum. */
void Demo_UpdateChecksum(void) {
    DemoPlayer *s = &D_800F56B0;
    s32 i;

    for (i = 0; i < s->playerCount; i++) {
        s->checksum += D_800B6D20 * s->buttons[i].value * 0x12345678;
        s->checksum += D_800B6D20 * s->stickX[i].value * 0x87654321;
        s->checksum += D_800B6D20 * s->stickY[i].value * 0x12348765;
    }
    s->checksum += D_800B6D20 * s->match.value * 0x87651234;
}
