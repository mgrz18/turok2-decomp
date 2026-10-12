#include "common.h"
#include "m2c_macros.h"
#include "structs/player.h"

extern f32 D_800B6D28;

void func_00254704(CPlayer *arg0) {
    f32 temp_f0;
    f32 temp_f1;

    temp_f1 = arg0->m_InvincibilityTime;
    if (temp_f1 > 0.0f) {
        temp_f0 = temp_f1 - D_800B6D28;
        arg0->m_InvincibilityTime = temp_f0;
        if (temp_f0 < 0.0f) {
            arg0->m_InvincibilityTime = 0.0f;
        }
    }
}
