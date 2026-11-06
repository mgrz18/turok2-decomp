#include "common.h"
#include "structs/game_object.h"

void EnemyAI_SetBehaviorIndex(s32 arg0, CAI *ai, s8 arg2) {
    ((s8 *) &ai->m_VarBuffer[9])[3] = arg2;
}
