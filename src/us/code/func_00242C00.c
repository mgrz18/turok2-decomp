#include "common.h"
#include "structs/game_object.h"

typedef f32 Mtx4[4][4];

typedef struct Dest {
    u32 flags;         /* 0x00 */
} Dest;

extern char D_800F1E10[];

CGameObjectInstance *func_00267800(CGameObjectInstance *actor);
void func_00268798(CGameObjectInstance *mover, CVector3 *move);
f32 func_002687FC(CGameObjectInstance *mover);
void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
void func_002105A4(Mtx4 *mtx, f32 angle);
void func_0020F908(Mtx4 *mtx, CVector3 *in, CVector3 *out, s32 count);
void func_00239F00(CGameObjectInstance *actor, CVector3 pos, void *arg);
void func_002640D4(CGameObjectInstance *mover, CGameObjectInstance *actor);

/* Carries an actor standing on a moving platform along with it. */
void func_00242C00(CGameObjectInstance *actor, Dest *dst) {
    CVector3 vel;
    CVector3 pos;
    CVector3 delta;
    Mtx4 mtx;
    CGameObjectInstance *mover;
    f32 turn;

    mover = func_00267800(actor);
    if (mover != NULL) {
        if (mover->header.m_pIntelligence->m_Type == 2) {
            dst->flags = (dst->flags & ~0x80000) | 0x40000;
            pos = actor->header.m_vPos;
            func_00268798(mover, &vel);
            if (!(dst->flags & 0x80) || vel.y > 0.0f) {
                func_0020EEF8(&pos, &pos, &vel);
            }
            vel = actor->m_vVelocity;
            actor->m_vVelocity.x = 0.0f;
            actor->m_vVelocity.y = 0.0f;
            actor->m_vVelocity.z = 0.0f;
            func_0020EF2C(&delta, &pos, &mover->header.m_vPos);
            turn = func_002687FC(mover);
            func_002105A4(&mtx, turn);
            func_0020F908(&mtx, &delta, &delta, 1);
            func_0020EEF8(&delta, &delta, &mover->header.m_vPos);
            func_0020EF2C(&delta, &delta, &pos);
            actor->m_RotY += turn;
            func_0020EEF8(&pos, &pos, &delta);
            func_00239F00(actor, pos, D_800F1E10);
            actor->m_vVelocity = vel;
            if (mover->header.m_Type == 1 && mover->header.m_pIntelligence->m_Type == 2) {
                func_002640D4(mover, actor);
            }
        } else {
            dst->flags = (dst->flags & ~0x40000) | 0x80000;
        }
    } else {
        dst->flags &= ~0xC0000;
    }
}
