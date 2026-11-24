#include "common.h"
#include "structs/game_object.h"
#include "protos/enemy_ai.h"



void EnemyMode_CloakActivate_Enter(CGameObjectInstance *arg0, CAI *arg1) {
    f32 temp_f20;
    f32 temp_f21;

    temp_f20 = 1.5e2f;
    temp_f21 = 3.0e2f;
    arg1->m_Enemy.m_Timer = Random_Blend(temp_f20, temp_f21);
    arg1->m_Enemy.m_Timer2 = Random_Blend(temp_f20, temp_f21);
}
