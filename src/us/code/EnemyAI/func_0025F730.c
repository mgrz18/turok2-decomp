#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


void EnemyMode_AirToUnderwater_Update(sptr arg0, CAI *ai) {
    ((s8 *)&ai->m_VarBuffer[9])[3] = 2;
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, ai, 4);
    }
}
