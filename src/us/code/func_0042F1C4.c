#include "common.h"
#include "structs/demo.h"

extern s32 gFrameCountGameplay;
extern f32 gFrameIncrement;
extern s32 gMergedController_buttonsSingle;

void func_00430330(void *);
void CEngineApp__SetupFadeTo(void *, s32);

/* Demo_UpdateChecksum */
static __inline__ void updateChecksum(void) {
    DemoPlayer *s = &gDemoPlayer;
    s32 i;

    for (i = 0; i < s->playerCount; i++) {
        s->checksum += gFrameCountGameplay * s->buttons[i].value * 0x12345678;
        s->checksum += gFrameCountGameplay * s->stickX[i].value * 0x87654321;
        s->checksum += gFrameCountGameplay * s->stickY[i].value * 0x12348765;
    }
    s->checksum += gFrameCountGameplay * s->match.value * 0x87651234;
}

void Demo_Update(void) {
    DemoPlayer *s = &gDemoPlayer;

    s->fastForward = 0;
    s->time += gFrameIncrement;
    if (s->done == 0) {
        if (--s->countdown <= 0 || gMergedController_buttonsSingle != 0) {
            s->done = 1;
            func_00430330(&gEngineApp);
            if (gMergedController_buttonsSingle != 0) {
                CEngineApp__SetupFadeTo(&gEngineApp, 8);
            }
        }
    }
    updateChecksum();
}
