#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);

void Throwable_Advance(s32 arg0, CAI *arg1) {
    arg1->m_VarBuffer[1] = 0;
    arg1->m_VarBuffer[2] = 0;
    CAI__SetMode(arg0, (s32) arg1, 0);
}
