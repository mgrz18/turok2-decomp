#include "common.h"
#include "structs/game_object.h"

f32 Random_Blend(f32 lo, f32 hi);

void EnemySwim_PuffUpEnter(CGameObjectInstance *self, CAI *ai) {
    self->m_dwFlags &= ~0x20000;
    ai->m_ConditionTimer = Random_Blend(30.0f, 60.0f);
}
