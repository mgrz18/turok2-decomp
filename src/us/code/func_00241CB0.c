#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"
#include "structs/region.h"

void func_00241960(CGameObjectInstance *mover, CVector3 *points, CGameObjectInstance *collider);
void func_0023C9CC(CollisionQuad *quad, CVector3 *points);
s32 func_0023C730(CollisionQuad *quad, f32 x, f32 z);
f32 func_0023C8A0(CollisionQuad *quad, f32 x, f32 z);

/* Lands the collider on top of the box face of a mover when it is within reach. */
void func_00241CB0(Sweep *ctrl, CGameObjectInstance *collider) {
    CGameObjectInstance *mover;
    CPlatformIntelligence *info;
    CVector3 points[8];
    CollisionQuad quad;
    f32 top;
    f32 y;
    f32 reach;

    mover = (CGameObjectInstance *)ctrl->obj;
    info = &mover->header.m_pIntelligence->m_Platform;
    if (info->m_CollisionType == 0 && collider->header.m_pCurrentRegion != NULL && mover->header.m_pCurrentRegion->m_nInfo != collider->header.m_pCurrentRegion->m_nInfo) {
        return;
    }
    func_00241960(mover, points, collider);
    if (!(ctrl->delta.y > 0.0f)) {
        return;
    }
    func_0023C9CC(&quad, points);
    if (info->m_CollisionType != 0 && !func_0023C730(&quad, collider->header.m_vPos.x, collider->header.m_vPos.z)) {
        return;
    }
    top = func_0023C8A0(&quad, collider->header.m_vPos.x, collider->header.m_vPos.z);
    y = collider->header.m_vPos.y;
    reach = top + ctrl->delta.y;
    if (!(top <= y) && info->m_CollisionType != 0) {
        return;
    }
    if (y <= reach) {
        collider->m_pInstanceBelow = mover;
        collider->header.m_vPos.y = top + 0.4096f;
        if (collider->m_vVelocity.y < 0.0f) {
            collider->m_vVelocity.y = 0.0f;
        }
        mover->m_AI.m_dwFlags |= 0x10000;
        if (collider->header.m_Type == 1 && (collider->m_dwFlags & 0x300000)) {
            mover->m_AI.m_dwFlags |= 0x20000;
        }
    }
}
