#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/game_object.h"
#include "globals/collision.h"
#include "protos/mtxf.h"



void CMtxF__TransformPoints(CMtxF mf, CVector3 *in, CVector3 *out, s32 count);
void Platform_IdleEnter(CGameObjectInstance *mover, CGameObjectInstance *actor);

/* Carries an actor standing on a moving platform along with it. */
void AI_RideMovingPlatform(CGameObjectInstance *actor, CAI *dst) {
    CVector3 vel;
    CVector3 pos;
    CVector3 delta;
    CMtxF mtx;
    CGameObjectInstance *mover;
    f32 turn;

    mover = GameObject_GetPlatformBelow(actor);
    if (mover != NULL) {
        if (mover->header.m_pIntelligence->m_Type == 2) {
            dst->m_dwFlags = (dst->m_dwFlags & ~0x80000) | 0x40000;
            pos = actor->header.m_vPos;
            GameObject_GetFrameDelta(mover, &vel);
            if (!(dst->m_dwFlags & 0x80) || vel.y > 0.0f) {
                CVector3__Add(&pos, &pos, &vel);
            }
            vel = actor->m_vVelocity;
            actor->m_vVelocity.x = 0.0f;
            actor->m_vVelocity.y = 0.0f;
            actor->m_vVelocity.z = 0.0f;
            CVector3__Subtract(&delta, &pos, &mover->header.m_vPos);
            turn = GameObject_GetFrameTurn(mover);
            CMtxF__SetRotateY(mtx, turn);
            CMtxF__TransformPoints(mtx, &delta, &delta, 1);
            CVector3__Add(&delta, &delta, &mover->header.m_vPos);
            CVector3__Subtract(&delta, &delta, &pos);
            actor->m_RotY += turn;
            CVector3__Add(&pos, &pos, &delta);
            Collision_MoveInstance(actor, pos, &gCollisionInfoPlatformRider);
            actor->m_vVelocity = vel;
            if (mover->header.m_Type == 1 && mover->header.m_pIntelligence->m_Type == 2) {
                Platform_IdleEnter(mover, actor);
            }
        } else {
            dst->m_dwFlags = (dst->m_dwFlags & ~0x40000) | 0x80000;
        }
    } else {
        dst->m_dwFlags &= ~0xC0000;
    }
}
