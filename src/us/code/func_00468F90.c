/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0044BAC0.c, func_0044BAC0, with Turok 2's layouts. */
/* Second VM module copy of Cont_InitAll (same source, linked 0xE0 lower). */
#include "common.h"
#include "structs/controller.h"


/* Controller / rumble slot, 0x224 bytes (Controller in Controller_ResetRumble). */

extern char gRumblePool[];
extern char gRumbleEntries[];
extern s8 gPakQueueState;     /* pak queue state */
extern char gPakAccessQueue[]; /* pak OSMesgQueue */
extern char gSiMessageQueue[]; /* SI OSMesgQueue */
extern char gSiMesgBuf[];
extern char gPakMesgBuf[];
extern u8 gValidControllerBits;     /* controller bit pattern */
extern s16 gFirstControllerPort;    /* lowest port with a controller */
extern u8 gNumControllers;     /* number of controllers */
extern u8 gControllersInitialised;     /* controllers initialised */
extern s32 gPakChanged[];
extern s32 gPakRemoved[];
extern s32 gPakInserted[];

s32 RumblePool_Init(void *dst, void *src, s32 n);
void osCreateMesgQueue(void *mq, void *msgBuf, s32 count); /* osCreateMesgQueue */
void osSetEventMesg(s32 event, void *mq, s32 msg);     /* osSetEventMesg */
s32 osContInit(void *mq, u8 *bitpattern, OSContStatus *status); /* osContInit */
s32 osSendMesg(void *mq, void *msg, s32 flag);
void Controller_UnwatchPak(s32);
s32 Cont_ResetSlot_2(ControllerProfile *, s32);

/* Initialises the controllers; returns the lowest port with one plugged in, or -1. */
s16 Cont_InitAll_2(void) {
    s32 i;

    RumblePool_Init(gRumblePool, gRumbleEntries, 8);
    gPakQueueState = 2;
    osCreateMesgQueue(gSiMessageQueue, gSiMesgBuf, 1);
    osCreateMesgQueue(gPakAccessQueue, gPakMesgBuf, 1);
    osSetEventMesg(5, gSiMessageQueue, 1);
    osContInit(gSiMessageQueue, &gValidControllerBits, gContStatus);
    osSendMesg(gPakAccessQueue, 0, 0);
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
