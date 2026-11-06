#include "common.h"

void CParticleSystem__Free(void *, s32);              /* extern */
void CParticle__StopLoopSound(s32);                         /* extern */
extern u8 D_800F8928[];

void CParticle__Kill(s32 arg0) {
    CParticleSystem__Free(D_800F8928, arg0);
    CParticle__StopLoopSound(arg0);
}
