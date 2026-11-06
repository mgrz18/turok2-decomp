#include "common.h"
#include "structs/demo.h"

extern s32 D_800B6D20;
extern f32 gFrameIncrement;
extern s32 D_800F5D2C;

void func_00430330(void *);
void CEngineApp__SetupFadeTo(void *, s32);

/* Demo_UpdateChecksum */
static __inline__ void updateChecksum(void) {
    DemoPlayer *s = &D_800F56B0;
    s32 i;

    for (i = 0; i < s->playerCount; i++) {
        s->checksum += D_800B6D20 * s->buttons[i].value * 0x12345678;
        s->checksum += D_800B6D20 * s->stickX[i].value * 0x87654321;
        s->checksum += D_800B6D20 * s->stickY[i].value * 0x12348765;
    }
    s->checksum += D_800B6D20 * s->match.value * 0x87651234;
}

void Demo_Update(void) {
    DemoPlayer *s = &D_800F56B0;

    s->fastForward = 0;
    s->time += gFrameIncrement;
    if (s->done == 0) {
        if (--s->countdown <= 0 || D_800F5D2C != 0) {
            s->done = 1;
            func_00430330(&gEngineApp);
            if (D_800F5D2C != 0) {
                CEngineApp__SetupFadeTo(&gEngineApp, 8);
            }
        }
    }
    updateChecksum();
}
