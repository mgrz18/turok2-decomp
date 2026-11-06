#include "common.h"
#include "globals/scene.h"

void CParticleSystem__Free(void *, s32);              /* extern */
void CParticle__StopLoopSound(s32);                         /* extern */

void CParticle__Kill(s32 arg0) {
    CParticleSystem__Free(gScene.m_ParticleSystem, arg0);
    CParticle__StopLoopSound(arg0);
}
