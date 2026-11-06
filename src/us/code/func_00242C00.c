#include "common.h"
#include "structs/game_object.h"

typedef f32 Mtx4[4][4];

typedef struct Dest {
    u32 flags;         /* 0x00 */
} Dest;

extern char D_800F1E10[];

CGameObjectInstance *GameObject_GetPlatformBelow(CGameObjectInstance *actor);
void GameObject_GetFrameDelta(CGameObjectInstance *mover, CVector3 *move);
f32 GameObject_GetFrameTurn(CGameObjectInstance *mover);
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CMtxF__SetRotateY(Mtx4 *mtx, f32 angle);
void CMtxF__TransformPoints(Mtx4 *mtx, CVector3 *in, CVector3 *out, s32 count);
void Collision_MoveInstance(CGameObjectInstance *actor, CVector3 pos, void *arg);
void Platform_IdleEnter(CGameObjectInstance *mover, CGameObjectInstance *actor);

/* Carries an actor standing on a moving platform along with it. */
void AI_RideMovingPlatform(CGameObjectInstance *actor, Dest *dst) {
    CVector3 vel;
    CVector3 pos;
    CVector3 delta;
    Mtx4 mtx;
    CGameObjectInstance *mover;
    f32 turn;

    mover = GameObject_GetPlatformBelow(actor);
    if (mover != NULL) {
        if (mover->header.m_pIntelligence->m_Type == 2) {
            dst->flags = (dst->flags & ~0x80000) | 0x40000;
            pos = actor->header.m_vPos;
            GameObject_GetFrameDelta(mover, &vel);
            if (!(dst->flags & 0x80) || vel.y > 0.0f) {
                CVector3__Add(&pos, &pos, &vel);
            }
            vel = actor->m_vVelocity;
            actor->m_vVelocity.x = 0.0f;
            actor->m_vVelocity.y = 0.0f;
            actor->m_vVelocity.z = 0.0f;
            CVector3__Subtract(&delta, &pos, &mover->header.m_vPos);
            turn = GameObject_GetFrameTurn(mover);
            CMtxF__SetRotateY(&mtx, turn);
            CMtxF__TransformPoints(&mtx, &delta, &delta, 1);
            CVector3__Add(&delta, &delta, &mover->header.m_vPos);
            CVector3__Subtract(&delta, &delta, &pos);
            actor->m_RotY += turn;
            CVector3__Add(&pos, &pos, &delta);
            Collision_MoveInstance(actor, pos, D_800F1E10);
            actor->m_vVelocity = vel;
            if (mover->header.m_Type == 1 && mover->header.m_pIntelligence->m_Type == 2) {
                Platform_IdleEnter(mover, actor);
            }
        } else {
            dst->flags = (dst->flags & ~0x40000) | 0x80000;
        }
    } else {
        dst->flags &= ~0xC0000;
    }
}
