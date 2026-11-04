#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

extern f32 D_800C0548;
extern s32 D_8012FA04;

void func_00440858(CGameObjectInstance *arg0, CAI *arg1) {
    f32 *speed = &arg0->m_AI.m_AnimSpeed;

    if (arg1->m_AnimRepeatedCount != 0) {
        *speed = (f32) D_800C0548;
        D_8012FA04 = -1;
        func_00243414((s32) arg0, (s32) arg1, 0x13);
    }
}
