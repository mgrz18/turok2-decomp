#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"
#include "structs/region.h"

void func_00241960(CGameObjectInstance *mover, CVector3 *points, CGameObjectInstance *collider);
void func_0023C9CC(CollisionQuad *quad, CVector3 *points);
s32 func_0023C2B0(CollisionQuad *quad, CVector3 *pos, CVector3 *move);
s32 func_0023C478(CollisionQuad *quad, CVector3 *pos, CVector3 *move, CVector3 *hit);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
s32 func_0023AB18(Sweep *ctrl, CVector3 *normal, CollisionQuad *quad, s32 arg3, s32 arg4, f32 scale, s32 arg6);

/* Collision of the controller's collider against a ground-type (kind 0) mover. */
s32 func_00241E30(Sweep *ctrl, CGameObjectInstance *mover) {
    CGameObjectInstance *collider;
    CVector3 points[8];
    CollisionQuad quad;
    CVector3 hit;
    f32 scale;

    collider = (CGameObjectInstance *)ctrl->obj;
    if (collider->header.m_pCurrentRegion != NULL && mover->header.m_pCurrentRegion->m_nInfo != collider->header.m_pCurrentRegion->m_nInfo) {
        return 0;
    }
    func_00241960(mover, points, collider);
    func_0023C9CC(&quad, points);
    if (collider->m_pInstanceBelow == NULL || mover->header.m_vPos.y > collider->m_pInstanceBelow->header.m_vPos.y) {
        collider->m_pInstanceBelow = mover;
    }
    if (func_0023C2B0(&quad, &collider->header.m_vPos, &ctrl->end)) {
        if (!func_0023C478(&quad, &collider->header.m_vPos, &ctrl->end, &hit)) {
            return 0;
        }
        scale = func_002982F0(mover->m_AI.m_Platform.m_Pitch);
        scale *= func_002974C0(mover->m_AI.m_Platform.m_Pitch);
        if (func_0023AB18(ctrl, &quad.normal, &quad, 0, 2, scale, 0)) {
            mover->m_AI.m_dwFlags |= 0x10000;
            if (collider->header.m_Type == 1 && (collider->m_dwFlags & 0x300000)) {
                mover->m_AI.m_dwFlags |= 0x20000;
            }
            collider->m_pInstanceBelow = mover;
            if (collider->m_vVelocity.y < 0.0f) {
                collider->m_vVelocity.y = 0.0f;
            }
            return 1;
        }
    }
    return 0;
}
