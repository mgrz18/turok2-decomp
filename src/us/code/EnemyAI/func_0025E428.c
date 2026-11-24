#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy_ai.h"

s8 EnemyAI_GetBehaviorIndex(sptr arg0, CAI *ai) {
    return ((s8 *) &ai->m_VarBuffer[9])[3];
}
