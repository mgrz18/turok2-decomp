#include "common.h"
#include "globals/boss.h"

s32 AI_IsAimedAt(void *, void *, sptr, f32);

s32 MantidQueen_GetTurnSide(s32 a0, s32 a1, s32 a2) {
    s32 result;

    if (gMantidQueen.blink != 0) {
        result = AI_IsAimedAt((void *) a0, (void *) a1, a2, 0.785398245f);
    } else {
        result = 0;
    }
    return result;
}
