#include "structs/particle.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00283D24.c, func_00283D24.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

s32 CParticle__IsUnderWater(CParticle *);
void ImpactEvent_Send(CParticle *, CParticle *, s32, s32, CVector3, CEventValue);
void CParticle__SpawnChild(CParticle *, s32, s32, s32);
void Sound__Play(s16, f32, f32, f32, s32, s32);
void func_002322D8(CParticle *o) {
    CROMParticleEffect *par = o->m_pEffect;
    s32 k = CParticle__IsUnderWater(o) ? 12 : 10;
    s32 s2 = par->m_pImpact->m_ImpactParticleType[k];
    s32 a2 = par->m_pImpact->m_ImpactEventType[k];
    CEventValue r = par->m_pImpact->m_ImpactEventValue[k];
    s32 s3 = par->m_pImpact->m_ImpactSoundType[k];
    if (a2 != 0xFFFF) ImpactEvent_Send(o, o, a2, -1, o->header.m_vPos, r);
    if (s2 != 0xFFFF) CParticle__SpawnChild(o, s2, o->m_Alignment, 1);
    if (s3 != 0xFFFF) Sound__Play(s3, o->header.m_vPos.x, o->header.m_vPos.y, o->header.m_vPos.z, 0, -1);
}
