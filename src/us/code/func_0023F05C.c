#include "common.h"
#include "structs/level_actor.h"

s32 func_002247E4(s32, s32, s32);
s32 func_00227780(s32);
s32 func_0023E820(s32, s32, s32);
s32 func_00268C04(s32);

s32 func_0026E19C(s32, s32, s32);           /* extern */
extern s32 D_800F7078;

void func_0023F05C(s32 arg0, CPooledLevelActor *arg1) {
    u16 temp_v0;
    u16 temp_v1;

    temp_v1 = arg1->m_Actor.m_wFlags;
    if (temp_v1 & 8) {
        if (arg1->m_PoolFlags & 1) {
            goto block_4;
        }
    } else if (!(temp_v1 & 1)) {
block_4:
        if (func_0023E820(arg0, (s32) arg1, func_00268C04((s32) arg1)) != 0) {
            temp_v0 = arg1->m_Actor.m_wFlags | 1;
            arg1->m_Actor.m_wFlags = temp_v0;
            if (temp_v0 & 8) {
                func_00227780((s32) arg1);
                return;
            }
            func_002247E4((s32) &D_800F7078, (s32) arg1, 1);
            func_0026E19C(arg1->m_Actor.header.m_pCurrentRegion, 0x400, arg0);
        }
    }
}
