#include "structs/particle.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00283D24.c, func_00283D24.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

s32 CParticle__IsUnderWater(CParticle *);
void ImpactEvent_Send(void *src, void *dst, s32 effect, s32 id, CVector3 pos, CEventValue params);
void CParticle__SpawnChild(CParticle *, s32, s32, s32);
/* The position as three floats: the port's shim (port/game/t2abi.c), the N64 definition under its own name. */
#ifdef TARGET_PORT
void Sound__Play_f3(s32 id, f32 x, f32 y, f32 z, s32 follow, s32 owner);
#else
void Sound__Play_f3(s16 id, f32 x, f32 y, f32 z, s32 follow, s32 owner) __asm__("Sound__Play");
#endif
void CParticle__FireExpireEvent(CParticle *o) {
    CROMParticleEffect *par = o->m_pEffect;
    s32 k = CParticle__IsUnderWater(o) ? 12 : 10;
    s32 s2 = par->m_pImpact->m_ImpactParticleType[k];
    s32 a2 = par->m_pImpact->m_ImpactEventType[k];
    CEventValue r = par->m_pImpact->m_ImpactEventValue[k];
    s32 s3 = par->m_pImpact->m_ImpactSoundType[k];
    if (a2 != 0xFFFF) ImpactEvent_Send(o, o, a2, -1, o->header.m_vPos, r);
    if (s2 != 0xFFFF) CParticle__SpawnChild(o, s2, o->m_Alignment, 1);
    if (s3 != 0xFFFF) Sound__Play_f3(s3, o->header.m_vPos.x, o->header.m_vPos.y, o->header.m_vPos.z, 0, -1);
}
