#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);

void MantidQueen_UpdateMode13(s32 arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, (s32) arg1, 8);
    }
}
