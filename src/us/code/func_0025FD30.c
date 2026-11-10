#include "common.h"
#include "structs/game_object.h"

s32 EnemyMode_Move_Update(sptr, sptr);

void EnemyMode_WoundedArm_Update(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        EnemyMode_Move_Update(arg0, (sptr) arg1);
    }
}
