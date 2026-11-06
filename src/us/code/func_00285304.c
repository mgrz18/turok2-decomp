#include "common.h"
#include "structs/controller.h"

/* Per-controller rumble pak state. */

extern u8 D_800B6CF0;           /* service enabled */
extern u8 D_800F6750;           /* rescan request */
extern u8 D_800F6758[];         /* OSMesgQueue: pak access lock */
extern u8 D_800F6798[];         /* OSMesgQueue for the SI */

extern s32 osRecvMesg(void *queue, void *msg, s32 flags);
extern s32 osSendMesg(void *queue, void *msg, s32 flags);
extern s32 osMotorInit(void *queue, void *pfs, s32 channel);
extern void Controller_ReadAll(void);
extern void RumblePool_Init(void *, void *, s32);
extern void osMotorStop(void *);

static __inline__ void ResetPadMotor(ControllerProfile *pad) {
    osRecvMesg(D_800F6758, NULL, 1);
    Controller_ReadAll();
    D_800F6750 = 2;
    pad->rumbleOn = 0;
    pad->rumbleLevel = 0;
    pad->rumbleTimer = 0;
    RumblePool_Init(pad->rumblePool, pad->rumbleNodes, 3);
    osMotorInit(D_800F6798, pad->rumblePfs, pad->port);
    osMotorStop(pad->rumblePfs);
    osMotorStop(pad->rumblePfs);
    if (osMotorInit(D_800F6798, pad->rumblePfs, pad->port) == 0) {
        pad->rumblePak = 1;
    }
    D_800F6750 = 2;
    osSendMesg(D_800F6758, NULL, 0);
}

void Controller_ResetAllRumble(void) {
    ControllerProfile *pad;
    s32 i;

    for (i = 0; i < 4; i++) {
        pad = &D_800F5EC0[i];
        if (D_800B6CF0 != 0 && pad->connected != 0) {
            ResetPadMotor(pad);
        }
    }
}
