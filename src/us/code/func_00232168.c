#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"

typedef struct { f32 m[4][4]; } Mtx4;

extern CCollisionResults D_800F1CE0;  /* the last collision test: the spawn is placed at its hit */
void func_0020D908(Mtx4 *out, void *src);
void func_0020C208(Mtx4 *out, Mtx4 *in);
void func_0020F85C(Mtx4 *m, CVector3 *in, CVector3 *out);
void func_0020F040(CVector3 *v);
void func_0020FAFC(Mtx4 *m, CVector3 *in, CVector3 *out);
void func_0025E52C(u8 *owner, u8 *ai, void *pos, s32 n);

void func_00232168(CParticle *actor, Mtx4 *mtx, s32 kind) {
    Mtx4 tmp;
    Mtx4 m;
    CVector3 dir;
    u8 *owner;

    owner = (u8 *)D_800F1CE0.m_pInstance;
    if (mtx != NULL) {
        func_0020C208(&m, mtx);
    } else {
        func_0020D908(&tmp, *(u8 **) (owner + 0x98) + D_800F1CE0.m_nInstanceNode * 0x40);
        func_0020C208(&m, &tmp);
    }
    func_0020F85C(&m, &D_800F1CE0.m_vInstancePos, &actor->m_vAttachPos);
    dir = D_800F1CE0.m_vVelocity;
    func_0020F040(&dir);
    func_0020FAFC(&m, &dir, &actor->m_vVelocity);
    actor->m_pTarget = (CGameObjectInstance *) owner;
    actor->m_dwFlags |= 0x10000;
    actor->m_nBone = ((u8 *)&D_800F1CE0.m_nInstanceNode)[3] /* its low byte */;
    if (**(s32 **) (owner + 0x14) == 1) {
        if (actor->m_pOwner != NULL) {
            func_0025E52C(owner, owner + 0x140, &actor->m_pOwner->header.m_vPos, 3);
        } else {
            func_0025E52C(owner, owner + 0x140, &D_800F1CE0.m_vInstancePos, 3);
        }
    }
    if (actor->header.m_nObjType == 0x56 && kind == 2) {
        actor->m_dwFlags |= 0x04000000;
    }
}
