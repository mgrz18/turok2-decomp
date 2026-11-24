/* The second VM module's copy of src/us/vm1/Demo/func_0042F868.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/demo.h"
#include "globals/engine.h"


/* Folds the timers' values into the state's checksum. */
void func_00468788(void) {
    DemoPlayer *s = &gDemoPlayer;
    s32 i;

    for (i = 0; i < s->playerCount; i++) {
        s->checksum += gFrameCountGameplay * s->buttons[i].value * 0x12345678;
        s->checksum += gFrameCountGameplay * s->stickX[i].value * 0x87654321;
        s->checksum += gFrameCountGameplay * s->stickY[i].value * 0x12348765;
    }
    s->checksum += gFrameCountGameplay * s->match.value * 0x87651234;
}
