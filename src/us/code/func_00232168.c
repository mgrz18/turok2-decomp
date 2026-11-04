#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

typedef struct { f32 m[4][4]; } Mtx4;

typedef struct {
    u8 pad0[0x18];
    /* 0x018 */ CVector3 dir;
    u8 pad24[0x10];
    /* 0x034 */ CVector3 pos;
    /* 0x040 */ u32 flags;
    u8 pad44[0xC8];
    /* 0x10C */ u8 *parent;
    u8 pad110[0x4];
    /* 0x114 */ u8 *owner;
    u8 pad118[0x95];
    /* 0x1AD */ u8 b1AD;
} Actor;

extern CCollisionResults D_800F1CE0;  /* the last collision test: the spawn is placed at its hit */
void func_0020D908(Mtx4 *out, void *src);
void func_0020C208(Mtx4 *out, Mtx4 *in);
void func_0020F85C(Mtx4 *m, CVector3 *in, CVector3 *out);
void func_0020F040(CVector3 *v);
void func_0020FAFC(Mtx4 *m, CVector3 *in, CVector3 *out);
void func_0025E52C(u8 *owner, u8 *ai, void *pos, s32 n);

void func_00232168(Actor *actor, Mtx4 *mtx, s32 kind) {
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
    func_0020F85C(&m, &D_800F1CE0.m_vInstancePos, &actor->pos);
    dir = D_800F1CE0.m_vVelocity;
    func_0020F040(&dir);
    func_0020FAFC(&m, &dir, &actor->dir);
    actor->owner = owner;
    actor->flags |= 0x10000;
    actor->b1AD = ((u8 *)&D_800F1CE0.m_nInstanceNode)[3] /* its low byte */;
    if (**(s32 **) (owner + 0x14) == 1) {
        if (actor->parent != NULL) {
            func_0025E52C(owner, owner + 0x140, actor->parent + 4, 3);
        } else {
            func_0025E52C(owner, owner + 0x140, &D_800F1CE0.m_vInstancePos, 3);
        }
    }
    if (*(u16 *) ((u8 *) actor + 2) == 0x56 && kind == 2) {
        actor->flags |= 0x04000000;
    }
}
