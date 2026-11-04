#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

void func_0025FD60(s32 arg0, CAI *ai) {
    s32 var_a2;

    if (ai->m_AnimRepeatedCount != 0) {
        var_a2 = 0x3C;
        if (ai->m_Health != 0) {
            var_a2 = 0x1E;
        }
        func_00243414(arg0, (s32) ai, var_a2);
    }
}
