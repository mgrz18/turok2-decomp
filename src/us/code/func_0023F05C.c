#include "common.h"

/* A level actor spawned at run time (a pickup), kept in the pool of the world (PoolActor in func_00227240). */
typedef struct PoolActor {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ s32 unk10;
    /* 0x014 */ u8 pad14[0x16C - 0x14];
    /* 0x16C */ u16 flags;
    /* 0x16E */ u8 pad16E[0x1A0 - 0x16E];
    /* 0x1A0 */ s32 poolFlags; /* 1: in use */
} PoolActor;

s32 func_002247E4(s32, s32, s32);
s32 func_00227780(s32);
s32 func_0023E820(s32, s32, s32);
s32 func_00268C04(s32);

s32 func_0026E19C(s32, s32, s32);           /* extern */
extern s32 D_800F7078;

void func_0023F05C(s32 arg0, PoolActor *arg1) {
    u16 temp_v0;
    u16 temp_v1;

    temp_v1 = arg1->flags;
    if (temp_v1 & 8) {
        if (arg1->poolFlags & 1) {
            goto block_4;
        }
    } else if (!(temp_v1 & 1)) {
block_4:
        if (func_0023E820(arg0, (s32) arg1, func_00268C04((s32) arg1)) != 0) {
            temp_v0 = arg1->flags | 1;
            arg1->flags = temp_v0;
            if (temp_v0 & 8) {
                func_00227780((s32) arg1);
                return;
            }
            func_002247E4((s32) &D_800F7078, (s32) arg1, 1);
            func_0026E19C(arg1->unk10, 0x400, arg0);
        }
    }
}
