#include "common.h"
#include "structs/boss.h"

extern u8 D_00430C6C[];
void ScriptAnimator_SetScript(void *slot, void *arg);

void MantidQueen_EnterMode15(void) {
    WaveBossState *s = &D_8012F3A0;
    s32 index = s->step->weakSpot;

    if (index != -1) {
        ScriptAnimator_SetScript(&s->spots[index].anim, D_00430C6C);
    }
}
