#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);
extern f32 D_800B6D28;

void func_00264740(CGameObjectInstance *arg0, CAI *arg1) {
    s32 temp_a3;
    s32 var_v1;
    CPlatformIntelligence *temp_t0;

    temp_t0 = &(arg0->header.m_pIntelligence)->m_Platform;
    temp_a3 = temp_t0->m_dwPlatformFlags;
    var_v1 = 1;
    if (temp_a3 & 0x20000) {
        var_v1 = (u32)(arg1->m_dwFlags & 0x20000) < (u32)var_v1;
    }
    if ((temp_a3 & 0x8000) && !(arg1->m_dwFlags & 0x20000)) {
        var_v1 = 0;
    }
    if (var_v1 != 0) {
        arg1->m_ConditionTimer = arg1->m_ConditionTimer - (D_800B6D28 / temp_t0->m_MoveTime);
    }
    if (arg1->m_ConditionTimer <= 0.0f) {
        func_00243414((s32) arg0, (s32) arg1, 0);
    }
}
