#include "common.h"
#include "structs/game_object.h"
#include "globals/boss.h"
#include "protos/ai.h"


void Primagen_UpdateMode21_2(CGameObjectInstance *arg0, CAI *arg1) {
    f32 *animSpeed = &arg0->m_AI.m_AnimSpeed;

    if (arg1->m_AnimRepeatedCount != 0) {
        *animSpeed = 1.0f;
        gLevelScript.script.current = -1;
        CAI__SetMode(arg0, arg1, 0x13);
    }
}
