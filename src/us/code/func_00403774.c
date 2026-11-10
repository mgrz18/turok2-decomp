#include "common.h"
#include "globals/boss.h"

s32 AI_IsAimedAt(s32 a0, sptr a1, s32 a2, f32 a3);

s32 MantidQueen_GetTurnSide(s32 a0, s32 a1, s32 a2) {
    s32 result;

    if (gMantidQueen.blink != 0) {
        result = AI_IsAimedAt(a0, a1, a2, 0.785398245f);
    } else {
        result = 0;
    }
    return result;
}
