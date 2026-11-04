#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

void func_0025F380(s32 arg0, CAI *arg1) {
    s32 temp_a0;

    temp_a0 = arg1->m_VarBuffer[2];
    func_00243414(temp_a0, temp_a0 + 0x140, 1);
}
