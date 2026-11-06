#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

typedef void (*FaceFunc)(CollisionQuad *quad, CVector3 *points);

/* The side faces of a box: Quad_SetBoxTop, Quad_SetBoxSide3267, Quad_SetBoxSide4510, Quad_SetBoxSide0374, Quad_SetBoxSide2156. */
extern FaceFunc sBoxSideFaceBuilders[];

void Platform_GetCollisionBox(CGameObjectInstance *mover, CVector3 *points, CGameObjectInstance *collider);
void Quad_SetBoxBottom(CollisionQuad *quad, CVector3 *points);
s32 Quad_ContainsPointXZ(CollisionQuad *quad, f32 x, f32 z);
s32 Plane_SegmentCrosses(CollisionQuad *quad, CVector3 *pos, CVector3 *move);
s32 Plane_IntersectSegment(CollisionQuad *quad, CVector3 *pos, CVector3 *move, CVector3 *hit);
s32 Quad_ContainsPoint(CollisionQuad *quad, f32 x, f32 y, f32 z);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
s32 Sweep_HitPlane(Sweep *ctrl, CVector3 *normal, CollisionQuad *quad, s32 arg3, s32 arg4, f32 scale, s32 arg6);

/* Platform_HitFace, inlined. */
static __inline__ s32 LandOn(Sweep *ctrl, CGameObjectInstance *mover, CGameObjectInstance *collider, CollisionQuad *quad) {
    f32 scale;

    scale = func_002982F0(mover->m_AI.m_Platform.m_Pitch);
    scale *= func_002974C0(mover->m_AI.m_Platform.m_Pitch);
    if (Sweep_HitPlane(ctrl, &quad->normal, quad, 0, 2, scale, 0) != 0) {
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
    return 0;
}

/* Collision of the controller's collider against a box-type (kind 2) mover. */
s32 Platform_CollideBox(Sweep *ctrl, CGameObjectInstance *mover) {
    CGameObjectInstance *collider;
    CVector3 points[8];
    CollisionQuad quad;
    CVector3 hit;
    s32 result;
    s32 i;

    collider = (CGameObjectInstance *)ctrl->obj;
    result = 0;
    Platform_GetCollisionBox(mover, points, collider);
    i = 0;
    while (sBoxSideFaceBuilders[i] != NULL) {
        sBoxSideFaceBuilders[i++](&quad, points);
        if (Plane_SegmentCrosses(&quad, &collider->header.m_vPos, &ctrl->end)
            && Plane_IntersectSegment(&quad, &collider->header.m_vPos, &ctrl->end, &hit)
            && Quad_ContainsPoint(&quad, hit.x, hit.y, hit.z)) {
            result |= Sweep_HitPlane(ctrl, &quad.normal, &quad, 1, (i == 0) ? 2 : ctrl->def->m_InstanceBehavior, 0.0f, 0);
        }
    }
    Quad_SetBoxBottom(&quad, points);
    if (Quad_ContainsPointXZ(&quad, ctrl->end.x, ctrl->end.z)) {
        if (collider->m_pInstanceBelow == NULL || mover->header.m_vPos.y > collider->m_pInstanceBelow->header.m_vPos.y) {
            collider->m_pInstanceBelow = mover;
        }
    }
    if (Plane_SegmentCrosses(&quad, &collider->header.m_vPos, &ctrl->end)) {
        if (!Plane_IntersectSegment(&quad, &collider->header.m_vPos, &ctrl->end, &hit)) {
            return result;
        }
        if (!Quad_ContainsPoint(&quad, hit.x, hit.y, hit.z)) {
            return result;
        }
        result |= LandOn(ctrl, mover, collider, &quad);
    }
    return result;
}
