#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"

s32 func_0024E5F0(s32, s32, s32);
f32 func_00255940(s32, s32);

void func_00255884(CPlayer *arg0, CGameObjectInstance *arg1) {
    f32 temp_f2;

    temp_f2 = arg0->m_SideSpeed;
    if (((temp_f2 < 0.0f) && (arg0->m_ContState.m_FinalMoveX >= 0.0f)) || ((temp_f2 > 0.0f) && (arg0->m_ContState.m_FinalMoveX <= 0.0f)) || (arg0->m_ModeTime >= 1.5f)) {
        func_0024E5F0((s32) arg0, (s32) arg1, 7);
    } else {
        arg1->m_vVelocity.y = 307.19998f;
    }
    func_00255940((s32) arg0, (s32) arg1);
}
