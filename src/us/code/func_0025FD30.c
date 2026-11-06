#include "common.h"
#include "structs/game_object.h"

s32 EnemyMode_Move_Update(s32, s32);

void EnemyMode_WoundedArm_Update(s32 arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        EnemyMode_Move_Update(arg0, (s32) arg1);
    }
}
