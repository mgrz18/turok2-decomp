#include "common.h"
#include "structs/demo.h"
#include "globals/engine.h"


/* Folds the timers' values into the state's checksum. */
void Demo_UpdateChecksum(void) {
    DemoPlayer *s = &gDemoPlayer;
    s32 i;

    for (i = 0; i < s->playerCount; i++) {
        s->checksum += gFrameCountGameplay * s->buttons[i].value * 0x12345678;
        s->checksum += gFrameCountGameplay * s->stickX[i].value * 0x87654321;
        s->checksum += gFrameCountGameplay * s->stickY[i].value * 0x12348765;
    }
    s->checksum += gFrameCountGameplay * s->match.value * 0x87651234;
}
