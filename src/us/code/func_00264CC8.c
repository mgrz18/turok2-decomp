#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

void func_00264CC8(s32 arg0, CAI *arg1) {
    arg1->m_VarBuffer[1] = 0;
    arg1->m_VarBuffer[2] = 0;
    func_00243414(arg0, (s32) arg1, 0);
}
