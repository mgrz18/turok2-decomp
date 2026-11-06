#include "common.h"
#include "structs/controller.h"
#include "globals/pak.h"
#include "globals/controller.h"

/* Per-controller rumble pak state. */

extern u8 gControllersInitialised;           /* service enabled */

extern s32 osRecvMesg(void *queue, void *msg, s32 flags);
extern s32 osSendMesg(void *queue, void *msg, s32 flags);
extern s32 osMotorInit(void *queue, void *pfs, s32 channel);
extern void Controller_ReadAll(void);
extern void RumblePool_Init(void *, void *, s32);
extern void osMotorStop(void *);

static __inline__ void ResetPadMotor(ControllerProfile *pad) {
    osRecvMesg(&gPakAccessQueue, NULL, 1);
    Controller_ReadAll();
    gPakQueueState = 2;
    pad->rumbleOn = 0;
    pad->rumbleLevel = 0;
    pad->rumbleTimer = 0;
    RumblePool_Init(pad->rumblePool, pad->rumbleNodes, 3);
    osMotorInit(&gSiMessageQueue, pad->rumblePfs, pad->port);
    osMotorStop(pad->rumblePfs);
    osMotorStop(pad->rumblePfs);
    if (osMotorInit(&gSiMessageQueue, pad->rumblePfs, pad->port) == 0) {
        pad->rumblePak = 1;
    }
    gPakQueueState = 2;
    osSendMesg(&gPakAccessQueue, NULL, 0);
}

void Controller_ResetAllRumble(void) {
    ControllerProfile *pad;
    s32 i;

    for (i = 0; i < 4; i++) {
        pad = &gControllers[i];
        if (gControllersInitialised != 0 && pad->connected != 0) {
            ResetPadMotor(pad);
        }
    }
}
