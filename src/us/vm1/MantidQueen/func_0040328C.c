#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

void ScriptAnimator_SetScript(void *slot, void *arg);

void MantidQueen_EnterMode15(void) {
    WaveBossState *s = &gMantidQueen;
    s32 index = s->step->weakSpot;

    if (index != -1) {
        ScriptAnimator_SetScript(&s->spots[index].anim, sMantidQueenSpotOpenScript);
    }
}
