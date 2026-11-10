#include "common.h"
#include "structs/game_object.h"

s8 EnemyAI_GetBehaviorIndex(sptr arg0, CAI *ai) {
    return ((s8 *) &ai->m_VarBuffer[9])[3];
}
