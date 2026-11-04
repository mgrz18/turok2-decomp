#include "structs/particle.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00284D98.c, func_00284D98.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

void func_0020EF2C(CVector3 *, CVector3 *, CVector3 *);
extern const f32 D_800A642C;
void func_00233190(void *owner, CParticle *ref, CParticle **arr, s32 n) {
    s32 i;
    CVector3 d;
    for (i = 0; i < n; i++) {
        CParticle *o = arr[i];
        if (o->header.m_nObjType != 0x57) {
            func_0020EF2C(&d, &o->header.m_vPos, &ref->header.m_vPos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= D_800A642C) o->m_nFrames = 1;
        }
    }
}
