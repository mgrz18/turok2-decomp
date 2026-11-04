#include "common.h"
#include "structs/game_object.h"

void func_002602C4(s32 arg0, CAI *arg1, s32 arg2) {
    s32 v = arg1->m_Aggression;

    v += arg2;
    if (v >= 0x100) {
        v = 0xFF;
    }
    arg1->m_Aggression = v;
}
