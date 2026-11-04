#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"

extern CCollisionResults D_800F1CE0;
void func_0020F040(CVector3 *v);
f32 func_002119FC(f32 a, f32 b);
void func_0020EF60(CVector3 *out, CVector3 *in, f32 scale);
void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);

void func_00232044(CParticle *actor, s32 kind) {
    CVector3 dir;

    actor->m_dwFlags |= 0x20000;
    actor->header.m_vPos = D_800F1CE0.m_vInstancePos;
    actor->m_vVelocity = D_800F1CE0.m_vVelocity;
    dir = D_800F1CE0.m_vVelocity;
    func_0020F040(&dir);
    actor->m_vDirection = dir;
    func_0020EF60(&dir, &dir, func_002119FC(-49.92f, 0.0f));
    func_0020EEF8(&actor->header.m_vPos, &actor->header.m_vPos, &dir);
    actor->m_Shake = 0x7F;
    if (actor->header.m_nObjType == 0x56 && kind == 2) {
        actor->m_dwFlags |= 0x04000000;
    }
}
