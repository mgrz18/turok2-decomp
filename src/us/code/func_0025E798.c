#include "common.h"
#include "structs/game_object.h"

extern void func_00275544(s16 sound, CVector3 pos, s32, s32);

void func_0025E798(CGameObjectInstance *obj) {
    u16 temp_a0;

    temp_a0 = obj->header.m_pIntelligence->m_Enemy.m_StartSound;
    if (temp_a0 != 0) {
        func_00275544((s16) temp_a0, obj->header.m_vPos, 0, -1);
    }
}
