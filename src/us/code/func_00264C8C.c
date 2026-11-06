#include "common.h"
#include "structs/game_object.h"

void Throwable_SetModeAnim(s32 arg0, CAI *arg1, s32 arg2) {
    arg1->m_VarBuffer[0] = arg2;
}
