#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy.h"


void EnemyRam_MoveEnter(sptr arg0, CAI *ai) {
    ai->m_Action = 0;
    ai->m_ActionTime = AI_LerpByAggression(arg0, ai, 90.0f, 30.0f);
}
