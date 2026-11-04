#include "common.h"
#include "structs/game_object.h"

f32 func_002119FC(f32 lo, f32 hi);
void func_00261B78(CGameObjectInstance *self, CAI *ai);

void func_00261C38(CGameObjectInstance *self, CAI *ai) {
    ai->m_pfCollision = (void (*)(CGameObjectInstance *, void *)) func_00261B78;
    ai->m_ModeTime = func_002119FC(0.0f, 62.83186f);
    ai->m_Enemy.m_Pitch = 0.0f;
    ai->m_Enemy.m_TurnRate = 0.017453294f;
    ai->m_Enemy.m_BaseScale = self->m_vScale.x;
    ai->m_Enemy.m_WanderTimer = 0.0f;
}
