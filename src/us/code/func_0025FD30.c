#include "common.h"
#include "structs/game_object.h"

s32 func_0025D474(s32, s32);

void func_0025FD30(s32 arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        func_0025D474(arg0, (s32) arg1);
    }
}
