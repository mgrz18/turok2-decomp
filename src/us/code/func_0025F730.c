#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);

void EnemyMode_AirToUnderwater_Update(s32 arg0, CAI *ai) {
    ((s8 *)&ai->m_VarBuffer[9])[3] = 2;
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, (s32) ai, 4);
    }
}
