#include "common.h"
#include "structs/game_object.h"

s32 func_0025ED4C(s32, s32);

void func_00260060(CGameObjectInstance *obj, CAI *ai) {
    ai->m_pfDraw = func_0025ED4C;
    ai->m_OnFireTimer = 75.0f;
}
