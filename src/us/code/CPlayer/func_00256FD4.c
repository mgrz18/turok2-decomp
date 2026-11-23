#include "common.h"
#include "globals/scene.h"

s32 CDLightSet__Free(s32 *, sptr);              /* extern */

void Particle_FreeSlot(s32 arg0, s32 *arg1) {
    s32 temp_a1;

    temp_a1 = *arg1;
    if (temp_a1 != 0) {
        CDLightSet__Free((s32 *) &gScene.m_DLightSet, temp_a1);
        *arg1 = 0;
    }
}
