/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/80263760_7.CPP, func_80264248_de, with Turok 2's layouts. */
#include "common.h"

/* Controller profile, stride 0x224. */
typedef struct {
    s32 present;      /* 0x00 */
    s8 port;          /* 0x04 */
    u8 pad5[0xC3];
    s32 rumbleOk;     /* 0xC8 */
    s32 rumbleState;  /* 0xCC */
    s32 rumbleAmount; /* 0xD0 */
    s32 rumbleTimer;  /* 0xD4 */
    u8 motor[0x68];   /* 0xD8 OSPfs */
    u8 label[0x2C];   /* 0x140 */
    u8 label2[0xB8];  /* 0x16C */
} Controller;

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

void func_00285AE0(Controller *ctrl) {
    if (D_800B6CF0 != 0 && ctrl->present != 0) {
        osRecvMesg(&D_800F6758, 0, 1);
        func_002847E0();
        D_800F6750 = 2;
        ctrl->rumbleState = 0;
        ctrl->rumbleAmount = 0;
        ctrl->rumbleTimer = 0;
        func_0026EB3C(ctrl->label, ctrl->label2, 3);
        osMotorInit(&D_800F6798, ctrl->motor, ctrl->port);
        func_00298E10(ctrl->motor);
        func_00298E10(ctrl->motor);
        if (osMotorInit(&D_800F6798, ctrl->motor, ctrl->port) == 0) {
            ctrl->rumbleOk = 1;
        }
        D_800F6750 = 2;
        osSendMesg((u8 *) &D_800F6750 + 8, 0, 0); /* same queue, addressed from the state byte */
    }
}
