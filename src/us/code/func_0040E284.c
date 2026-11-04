#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

void func_0040E284(s32 arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        func_00243414(arg0, (s32) ai, 2);
    }
}
