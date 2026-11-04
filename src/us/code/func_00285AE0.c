/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/80263760_7.CPP, func_80264248_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/controller.h"

/* ControllerProfile profile, stride 0x224. */

s32 osRecvMesg(void *mq, void *msg, s32 flag);
s32 osSendMesg(void *mq, void *msg, s32 flag);
s32 osMotorInit(void *mq, void *pfs, s32 channel);
void func_002847E0(void);
s32 func_0026EB3C(void *dst, void *src, s32 n);
u32 func_00298E10(void *pfs);

extern u8 D_800B6CF0;  /* controllers initialised */
extern s8 D_800F6750;  /* pak queue state */
extern u8 D_800F6758;  /* pak OSMesgQueue */
extern u8 D_800F6798;  /* SI OSMesgQueue */

void func_00285AE0(ControllerProfile *ctrl) {
    if (D_800B6CF0 != 0 && ctrl->connected != 0) {
        osRecvMesg(&D_800F6758, 0, 1);
        func_002847E0();
        D_800F6750 = 2;
        ctrl->rumbleOn = 0;
        ctrl->rumbleLevel = 0;
        ctrl->rumbleTimer = 0;
        func_0026EB3C(ctrl->rumblePool, ctrl->rumbleNodes, 3);
        osMotorInit(&D_800F6798, ctrl->rumblePfs, ctrl->port);
        func_00298E10(ctrl->rumblePfs);
        func_00298E10(ctrl->rumblePfs);
        if (osMotorInit(&D_800F6798, ctrl->rumblePfs, ctrl->port) == 0) {
            ctrl->rumblePak = 1;
        }
        D_800F6750 = 2;
        osSendMesg((u8 *) &D_800F6750 + 8, 0, 0); /* same queue, addressed from the state byte */
    }
}
