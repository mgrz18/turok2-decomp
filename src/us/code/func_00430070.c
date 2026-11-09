/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0044BAC0.c, func_0044BAC0, with Turok 2's layouts. */
#include "common.h"
#include "structs/controller.h"
#include "globals/pak.h"
#include "globals/controller.h"
#include "globals/rumble.h"
#include "protos/ultra.h"

/* Controller / rumble slot, 0x224 bytes (Controller in Controller_ResetRumble). */


s32 RumblePool_Init(RumblePool *pool, RumbleEffect *effects, s32 count);
void Controller_UnwatchPak(s32);
s32 Cont_ResetSlot(ControllerProfile *, s32);

/* Initialises the controllers; returns the lowest port with one plugged in, or -1. */
s16 Cont_InitAll(void) {
    s32 i;

    RumblePool_Init(&gRumblePool, gRumbleEntries, 8);
    gPakQueueState = 2;
    osCreateMesgQueue(&gSiMessageQueue, &gSiMesgBuf, 1);
    osCreateMesgQueue(&gPakAccessQueue, gPakMesgBuf, 1);
    osSetEventMesg(5, &gSiMessageQueue, 1);
    osContInit(&gSiMessageQueue, &gValidControllerBits, gContStatus);
    osSendMesg(&gPakAccessQueue, 0, 0);
    gFirstControllerPort = -1;
    gNumControllers = 0;
    for (i = 3; i >= 0; i--) {
        gPakChanged[i] = 0;
        gPakRemoved[i] = 0;
        gPakInserted[i] = 0;
        Controller_UnwatchPak(i);
        if (Cont_ResetSlot(&gControllers[i], i) != 0) {
            gFirstControllerPort = i;
            gNumControllers++;
        }
    }
    gControllersInitialised = 1;
    return gFirstControllerPort;
}
