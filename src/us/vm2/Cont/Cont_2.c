/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0044BAC0.c, func_0044BAC0, with Turok 2's layouts. */
/* Second VM module copy of Cont_InitAll (same source, linked 0xE0 lower). */
#include "common.h"
#include "structs/controller.h"
#include "globals/pak.h"
#include "globals/controller.h"
#include "globals/rumble.h"
#include "protos/ultra.h"

/* Controller / rumble slot, 0x224 bytes (Controller in Controller_ResetRumble). */


s32 RumblePool_Init(RumblePool *pool, RumbleEffect *effects, s32 count);
void Controller_UnwatchPak(sptr);
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044BC58.c, func_0044BC58.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
s32 Cont_ResetSlot_2(S5Ctl *c, s32 port);

/* Initialises the controllers; returns the lowest port with one plugged in, or -1. */
s16 Cont_InitAll_2(void) {
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
        if (Cont_ResetSlot_2(&gControllers[i], i) != 0) {
            gFirstControllerPort = i;
            gNumControllers++;
        }
    }
    gControllersInitialised = 1;
    return gFirstControllerPort;
}

#include "structs/rumble.h"

s32 Cont_ResetSlot_2(S5Ctl *c, s32 port) {
    s32 i;
    c->port = port;
    c->f0 = 0;
    c->fC8 = 0;
    c->fCC = 0;
    c->f220 = 0;
    c->f8 = 0;
    c->fC = 0;
    c->f10 = 0;
    c->fC4 = 0;
    c->fC5 = 0;
    c->fC6 = 0;
    c->fC7 = 0;
    c->f14 = 0;
    c->f18 = 0;
    c->f1C = 0;
    c->f20 = 0;
    c->f24 = 0;
    c->f28 = 0;
    for (i = 0; i < 32; i++) {
        c->ent[i].a = 0;
        c->ent[i].b = 0;
        c->ent[i].c = 0x30;
        c->ent[i].d = 0;
    }
    c->f0 = ((gContStatus[port].errno >> 3) ^ 1) & 1;
    c->fD0 = 0;
    c->fD4 = 0;
    RumblePool_Init((RumblePool *) c->blkA, (RumbleEffect *) c->blkB, 3);
    return c->f0;
}
