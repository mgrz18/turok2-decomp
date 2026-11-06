#include "common.h"
#include "structs/game_object.h"
#include "globals/boss.h"

s32 CAI__SetMode(s32, s32, s32);

extern f32 D_800C0544;

void Primagen_UpdateMode21_2(CGameObjectInstance *arg0, CAI *arg1) {
    f32 *animSpeed = &arg0->m_AI.m_AnimSpeed;

    if (arg1->m_AnimRepeatedCount != 0) {
        *animSpeed = D_800C0544;
        gLevelScript.script.current = -1;
        CAI__SetMode((s32) arg0, (s32) arg1, 0x13);
    }
}
