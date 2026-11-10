#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

typedef struct Cylinder {
    f32 v[6];
} Cylinder;

void Platform_GetCollisionBox(CGameObjectInstance *mover, CVector3 *points, CGameObjectInstance *collider);
void Quad_SetBoxBottom(CollisionQuad *quad, CVector3 *points);
void Quad_SetBoxTop(CollisionQuad *quad, CVector3 *points);
s32 Quad_ContainsPointXZ(CollisionQuad *quad, f32 x, f32 z);
s32 Plane_SegmentCrosses(CollisionQuad *quad, CVector3 *pos, CVector3 *move);
s32 Plane_IntersectSegment(CollisionQuad *quad, CVector3 *pos, CVector3 *move, CVector3 *hit);
f32 GameObject_GetCollisionRadius(CGameObjectInstance *collider);
f32 GameObject_GetCollisionHeight(CGameObjectInstance *collider);
void Sphere_Set(Cylinder *cyl, CVector3 *center, f32 radius);
s32 Cylinder_ContainsPoint(Cylinder *cyl, CVector3 *point);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Sweep_HitCylinder Sweep_HitCylinder_i
#endif
s32 Sweep_HitCylinder(Sweep *ctrl, CVector3 *center, f32 radius, s32 arg3, f32 top, f32 bottom, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
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

/* Collision of the controller's collider against a cylinder-type (kind 1) mover. */
s32 Platform_CollideCylinder(Sweep *ctrl, CGameObjectInstance *mover) {
    CGameObjectInstance *collider;
    CIntelligence *def;
    CPlatformIntelligence *info;
    CVector3 points[8];
    CollisionQuad quad;
    Cylinder cyl;
    CVector3 hit;
    s32 result;
    f32 radius;
    f32 lift;
    f32 y;

    collider = (CGameObjectInstance *)ctrl->obj;
    def = mover->header.m_pIntelligence;
    Platform_GetCollisionBox(mover, points, collider);
    info = &def->m_Platform;
    Sphere_Set(&cyl, &mover->header.m_vPos, info->m_CollisionRadius + GameObject_GetCollisionRadius(collider));
    if (def->m_Platform.m_dwFlags & 1) {
        result = Sweep_HitCylinder(ctrl, &mover->header.m_vPos, info->m_CollisionRadius + GameObject_GetCollisionRadius(collider), 1,
                               mover->header.m_vPos.y + info->m_CollisionHeight, collider->header.m_vPos.y, 1, ctrl->def->m_InstanceBehavior, 0, 0);
    } else {
        radius = info->m_CollisionRadius + GameObject_GetCollisionRadius(collider);
        lift = GameObject_GetCollisionHeight(collider);
        y = mover->header.m_vPos.y;
        result = Sweep_HitCylinder(ctrl, &mover->header.m_vPos, radius, 1,
                               y + info->m_CollisionHeight, y - lift, 1, ctrl->def->m_InstanceBehavior, 0, 0);
    }
    Quad_SetBoxBottom(&quad, points);
    if (Quad_ContainsPointXZ(&quad, ctrl->end.x, ctrl->end.z)) {
        if (collider->m_pInstanceBelow == NULL || mover->header.m_vPos.y > collider->m_pInstanceBelow->header.m_vPos.y) {
            collider->m_pInstanceBelow = mover;
        }
    }
    if (Plane_SegmentCrosses(&quad, &collider->header.m_vPos, &ctrl->end)
        && Plane_IntersectSegment(&quad, &collider->header.m_vPos, &ctrl->end, &hit)
        && Cylinder_ContainsPoint(&cyl, &hit)) {
        result |= LandOn(ctrl, mover, collider, &quad);
    }
    Quad_SetBoxTop(&quad, points);
    if (Plane_SegmentCrosses(&quad, &collider->header.m_vPos, &ctrl->end)
        && Plane_IntersectSegment(&quad, &collider->header.m_vPos, &ctrl->end, &hit)
        && Cylinder_ContainsPoint(&cyl, &hit)) {
        result |= Sweep_HitPlane(ctrl, &quad.normal, &quad, 1, 2, 0.0f, 0);
    }
    return result;
}
