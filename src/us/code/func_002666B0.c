#include "common.h"
#include "structs/game_object.h"

s32 func_0021A7EC(s32);
s32 func_0021E9A4(s32, s32, s32);

void func_002666B0(s32 arg0, CAI *arg1, s32 arg2) {
    arg1->m_ModeLastFrame = -1;
    arg1->m_AnimRepeatedCount = 0;
    arg1->m_VarBuffer[0] = arg2;
    func_0021E9A4(arg0, arg2, 1);
    func_0021A7EC(arg0);
}
