#include "common.h"
#include "structs/game_object.h"
#include "rw/abu_protos.h"

f32 Random_Blend(f32 arg0, f32 arg1);

extern f32 D_800A790C;
extern f32 D_800A7910;

void EnemyMode_CloakActivate_Enter(s32 arg0, CAI *arg1) {
    f32 temp_f20;
    f32 temp_f21;

    temp_f20 = D_800A790C;
    temp_f21 = D_800A7910;
    arg1->m_Enemy.m_Timer = Random_Blend(temp_f20, temp_f21);
    arg1->m_Enemy.m_Timer2 = Random_Blend(temp_f20, temp_f21);
}
