/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0044BAC0.c, func_0044BAC0, with Turok 2's layouts. */
#include "common.h"
#include "structs/controller.h"
#include "globals/pak.h"
#include "globals/controller.h"
#include "globals/rumble.h"
#include "protos/ultra.h"

/* Controller / rumble slot, 0x224 bytes (Controller in Controller_ResetRumble). */


s32 RumblePool_Init(RumblePool *pool, RumbleEffect *effects, s32 count);
void Controller_UnwatchPak(sptr);
s32 Cont_ResetSlot(ControllerProfile *c, s32 port);

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

/* Controller / rumble slot, 0x224 bytes (Controller in Controller_ResetRumble). */

#include "structs/rumble.h"

/* Resets a controller slot; returns whether a controller is plugged into its port. */
s32 Cont_ResetSlot(ControllerProfile *c, s32 port) {
    s32 i;

    c->port = port;
    c->connected = 0;
    c->rumblePak = 0;
    c->rumbleOn = 0;
    c->reset = 0;
    c->unk8 = 0;
    c->unkC = 0;
    c->unk10 = 0;
    c->inputStickX = 0;
    c->inputStickY = 0;
    c->stickX = 0;
    c->stickY = 0;
    c->inputPreviousButtons = 0;
    c->inputButtons = 0;
    c->inputButtonsSingle = 0;
    c->inputButtonsSingleNoHold = 0;
    c->inputButtonsDouble = 0;
    c->inputButtonsRepeat = 0;
    for (i = 0; i < 32; i++) {
        c->buttonHistory[i].taps = 0;
        c->buttonHistory[i].age = 0;
        c->buttonHistory[i].repeat = 0x30;
        c->buttonHistory[i].held = 0;
    }
    c->connected = ((gContStatus[port].errno >> 3) ^ 1) & 1;
    c->rumbleLevel = 0;
    c->rumbleTimer = 0;
    RumblePool_Init((RumblePool *) c->rumblePool, (RumbleEffect *) c->rumbleNodes, 3);
    return c->connected;
}
