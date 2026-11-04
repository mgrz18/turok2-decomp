#include "common.h"
#include "structs/particle.h"

extern u8 D_800F8928[];
void func_0020E810(CQuaternion *out, CVector3 *dir);
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
void func_0020EF60(CVector3 *out, CVector3 *in, f32 scale);
void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);
s32 func_0022F350(void *table, void *actor, void *owner, s32 *ownerCount, void *target, s32 kind, CVector3 origin,
                  CQuaternion rot, CVector3 pos, void *region, s32 b, s32 c);

/* Spawns `count` copies along the segment the actor covered this frame. */
void func_00232934(CParticle *actor) {
    CQuaternion rot;
    CVector3 delta;
    CVector3 pos;
    CVector3 dir;
    s32 count;
    s32 kind;
    s32 i;
    u8 *table;

    count = actor->m_pEffect->m_pParams->m_nTrailCount;
    if (count > 0) {
        dir = actor->m_vVelocity;
        func_0020E810(&rot, &dir);
        func_0020EF2C(&delta, &actor->header.m_vPos, &actor->m_vLastPos);
        table = D_800F8928;
        for (i = 1; i <= count; i++) {
            func_0020EF60(&pos, &delta, i / (count + 1.0f));
            func_0020EEF8(&pos, &pos, &actor->m_vLastPos);
            kind = actor->header.m_nObjType;
            func_0022F350(table, actor, actor->m_pOwner, actor->m_pOwnerCount, actor->m_pTarget, kind, dir, rot,
                          pos, actor->header.m_pCurrentRegion, -1, 0x20E001);
        }
    }
}
