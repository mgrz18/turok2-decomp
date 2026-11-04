#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

void func_0025F6DC(s32 arg0, CAI *arg1) {
    arg1->m_VarS8[39] = 2;
    if (arg1->m_AnimRepeatedCount != 0) {
        func_00243414(arg0, (s32) arg1, 4);
    }
}
