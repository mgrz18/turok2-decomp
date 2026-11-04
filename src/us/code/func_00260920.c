#include "common.h"
#include "structs/game_object.h"

f32 func_002119FC(f32 lo, f32 hi);

void func_00260920(void *self, CAI *ai) {
    f32 lo = -40.96f;
    f32 hi = 40.96f;

    ai->m_vTargetOffset.x = func_002119FC(lo, hi);
    ai->m_vTargetOffset.y = func_002119FC(lo, hi);
    ai->m_vTargetOffset.z = func_002119FC(lo, hi);
}
