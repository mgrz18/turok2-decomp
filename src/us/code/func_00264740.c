#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"
#include "protos/ai.h"


void Platform_BackUpdate(CGameObjectInstance *arg0, CAI *arg1) {
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
        arg1->m_ConditionTimer = arg1->m_ConditionTimer - (gFrameIncrement / temp_t0->m_MoveTime);
    }
    if (arg1->m_ConditionTimer <= 0.0f) {
        CAI__SetMode(arg0, arg1, 0);
    }
}
