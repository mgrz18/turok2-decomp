#include "common.h"
#include "structs/game_object.h"

void EnemyAI_SetBehavior(s32 arg0, CAI *arg1, s32 arg2, s8 arg3) {
    arg1->m_Enemy.m_Behavior[arg2] = arg3;
}
