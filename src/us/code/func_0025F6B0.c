#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void EnemyMode_GroundToAir_Update(s32 arg0, CAI *arg1) {
    ((s8 *)arg1->m_VarBuffer)[0x133 - 0x10C] = 1;
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, (s32) arg1, 4);
    }
}
