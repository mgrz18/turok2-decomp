/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/80263760_7.CPP, func_80264248_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/controller.h"
#include "globals/pak.h"
#include "globals/controller.h"

/* ControllerProfile profile, stride 0x224. */

s32 osRecvMesg(void *mq, void *msg, s32 flag);
s32 osSendMesg(void *mq, void *msg, s32 flag);
s32 osMotorInit(void *mq, void *pfs, s32 channel);
void Controller_ReadAll(void);
s32 RumblePool_Init(void *dst, void *src, s32 n);
u32 osMotorStop(void *pfs);

void Controller_ResetRumble(ControllerProfile *ctrl) {
    if (gControllersInitialised != 0 && ctrl->connected != 0) {
        osRecvMesg(&gPakAccessQueue, 0, 1);
        Controller_ReadAll();
        gPakQueueState = 2;
        ctrl->rumbleOn = 0;
        ctrl->rumbleLevel = 0;
        ctrl->rumbleTimer = 0;
        RumblePool_Init(ctrl->rumblePool, ctrl->rumbleNodes, 3);
        osMotorInit(&gSiMessageQueue, ctrl->rumblePfs, ctrl->port);
        osMotorStop(ctrl->rumblePfs);
        osMotorStop(ctrl->rumblePfs);
        if (osMotorInit(&gSiMessageQueue, ctrl->rumblePfs, ctrl->port) == 0) {
            ctrl->rumblePak = 1;
        }
        gPakQueueState = 2;
        osSendMesg((u8 *) &gPakQueueState + 8, 0, 0); /* same queue, addressed from the state byte */
    }
}
