#include "common.h"

s32 CDLightSet__Free(s32 *, s32);              /* extern */
extern s32 D_8010FF34;

void Particle_FreeSlot(s32 arg0, s32 *arg1) {
    s32 temp_a1;

    temp_a1 = *arg1;
    if (temp_a1 != 0) {
        CDLightSet__Free(&D_8010FF34, temp_a1);
        *arg1 = 0;
    }
}
