#include "common.h"
#include "structs/particle.h"

typedef struct { f32 m[4][4]; } Mtx4;

/* The view the spawn is placed from (0x800F1CE8). */
typedef struct {
    /* 0x00 */ CVector3 pos;
    u8 padC[0x3];
    /* 0x0F */ u8 b0F;
    u8 pad10[0xC0];
    /* 0xD0 */ CVector3 dir;
} SpawnView;

typedef struct {
    /* 0x00 */ u8 *owner;
    u8 pad4[0x10];
    /* 0x14 */ s32 node;
} SpawnSource;

extern SpawnSource D_800F1CE0;
extern SpawnView D_800F1CE8;
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

    owner = D_800F1CE0.owner;
    if (mtx != NULL) {
        func_0020C208(&m, mtx);
    } else {
        func_0020D908(&tmp, *(u8 **) (owner + 0x98) + D_800F1CE0.node * 0x40);
        func_0020C208(&m, &tmp);
    }
    func_0020F85C(&m, &D_800F1CE8.pos, &actor->m_vAttachPos);
    dir = D_800F1CE8.dir;
    func_0020F040(&dir);
    func_0020FAFC(&m, &dir, &actor->m_vVelocity);
    actor->m_pTarget = (CGameObjectInstance *) owner;
    actor->m_dwFlags |= 0x10000;
    actor->m_nBone = D_800F1CE8.b0F;
    if (**(s32 **) (owner + 0x14) == 1) {
        if (actor->m_pOwner != NULL) {
            func_0025E52C(owner, owner + 0x140, &actor->m_pOwner->header.m_vPos, 3);
        } else {
            func_0025E52C(owner, owner + 0x140, &D_800F1CE8, 3);
        }
    }
    if (actor->header.m_nObjType == 0x56 && kind == 2) {
        actor->m_dwFlags |= 0x04000000;
    }
}
