#include "common.h"
#include "structs/game_object.h"

s32 InteractiveAnim_GetAnim(s32 arg0, CAI *arg1) {
    return arg1->m_VarBuffer[0];
}
