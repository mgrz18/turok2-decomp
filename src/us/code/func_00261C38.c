#include "common.h"
#include "structs/game_object.h"

f32 func_002119FC(f32 lo, f32 hi);
void func_00261B78(CGameObjectInstance *self, CAI *ai);

void func_00261C38(CGameObjectInstance *self, CAI *ai) {
    ai->m_pfCollision = (void (*)(CGameObjectInstance *, void *)) func_00261B78;
    ai->m_ModeTime = func_002119FC(0.0f, 62.83186f);
    ai->m_VarF32[0] = 0.0f;
    ai->m_VarF32[3] = 0.017453294f;
    ai->m_VarF32[4] = self->m_vScale.x;
    ai->m_VarF32[7] = 0.0f;
}
