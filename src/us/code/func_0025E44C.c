#include "common.h"
#include "structs/game_object.h"

/* The current mode entry. */
s8 EnemyAI_GetCurrentBehavior(sptr arg0, CAI *arg1) {
    return arg1->m_Enemy.m_Behavior[arg1->m_Enemy.m_BehaviorIndex];
}
