#include "common.h"
#include "structs/game_object.h"

s32 Throwable_SetModeAnim(s32, s32, s32);               /* extern */

void EnemyAI_SetGrabbed(s32 arg0, CAI *arg1, s32 arg2) {
    arg1->m_VarBuffer[2] = arg2;
    if (arg2 != 0) {
        Throwable_SetModeAnim(arg2, arg2 + 0x140, arg0);
    }
}
