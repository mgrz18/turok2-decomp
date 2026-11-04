#include "common.h"
#include "structs/game_object.h"

extern f32 func_0025EB50(s32, void *, f32, f32);

void func_00261128(s32 arg0, CAI *ai) {
    ai->m_Action = 0;
    ai->m_ActionTime = func_0025EB50(arg0, ai, 90.0f, 30.0f);
}
