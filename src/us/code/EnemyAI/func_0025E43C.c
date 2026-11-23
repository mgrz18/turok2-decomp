#include "common.h"
#include "structs/game_object.h"

s8 EnemyAI_GetBehavior(s32 arg0, CAI *arg1, s32 arg2) {
    return arg1->m_Enemy.m_Behavior[arg2];
}
