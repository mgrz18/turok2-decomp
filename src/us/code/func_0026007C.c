#include "common.h"
#include "structs/game_object.h"

s32 func_00246918(s32, s32);
s32 func_002600E8(s32, s32);

void func_0026007C(s32 arg0, CAI *arg1) {
    if (arg1->m_OnFireTimer <= 0.0f) {
        func_00246918(arg0, (s32) arg1);
    }
    if (arg1->m_ModeTime >= 150.0f) {
        func_002600E8(arg0, (s32) arg1);
    }
}
