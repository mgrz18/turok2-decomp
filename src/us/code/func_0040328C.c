#include "common.h"
#include "structs/boss.h"

extern u8 D_00430C6C[];
void func_00236314(void *slot, void *arg);

void func_0040328C(void) {
    WaveBossState *s = &D_8012F3A0;
    s32 index = s->step->weakSpot;

    if (index != -1) {
        func_00236314(&s->spots[index].anim, D_00430C6C);
    }
}
