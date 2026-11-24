#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy_ai.h"


void EnemyMode_WoundedArm_Update(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        EnemyMode_Move_Update(arg0, (sptr) arg1);
    }
}
