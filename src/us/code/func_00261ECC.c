#include "common.h"
#include "structs/game_object.h"

f32 func_002119FC(f32 lo, f32 hi);

void func_00261ECC(CGameObjectInstance *self, CAI *ai) {
    self->m_dwFlags &= ~0x20000;
    ai->m_ConditionTimer = func_002119FC(30.0f, 60.0f);
}
