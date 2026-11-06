#include "common.h"
#include "structs/game_object.h"

f32 Random_Blend(f32 lo, f32 hi);

void EnemyHover_Setup(void *self, CAI *ai) {
    f32 lo = -40.96f;
    f32 hi = 40.96f;

    ai->m_vTargetOffset.x = Random_Blend(lo, hi);
    ai->m_vTargetOffset.y = Random_Blend(lo, hi);
    ai->m_vTargetOffset.z = Random_Blend(lo, hi);
}
