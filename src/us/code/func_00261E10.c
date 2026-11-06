#include "common.h"
#include "structs/game_object.h"

extern f32 D_800A7B3C;
extern f32 Random_Blend(f32, f32);

void EnemySwim_IdleEnter(s32 arg0, CAI *arg1) {
    arg1->m_ConditionTimer = Random_Blend(0.0f, D_800A7B3C);
}
