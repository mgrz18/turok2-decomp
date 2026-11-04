#include "common.h"
#include "structs/player.h"

s32 func_00249358(s32, s32, s32, s32, s32);
s32 func_00254430(s32, s32);

extern s32 D_8011AAD8;

void func_00254584(CPlayer *arg0, CGameObjectInstance *arg1) {
    s32 var_v0;

    if (D_8011AAD8 & 0x10) {
        var_v0 = 1;
    } else {
        var_v0 = arg0->m_Data.m_SpecialPickupCount[380] & 0xF;
    }
    if (var_v0 != 0) {
        if (arg1->m_AI.m_dwFlags & 0x80) {
            func_00254430((s32) arg0, (s32) arg1);
        }
    } else {
        func_00249358((s32) arg0, (s32) arg1, 0xA00, 0x02000000, 0x1C);
    }
}
