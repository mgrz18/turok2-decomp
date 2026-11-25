#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "globals/math.h"
#include "protos/game.h"

typedef f32 Mtx4[4][4];
f32 CGameRegion__GetHeight(void *region, f32 x, f32 z);
f32 GameObject_GetCollisionRadius(CGameObjectInstance *collider);
f32 GameObject_GetCollisionHeight(CGameObjectInstance *collider);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void CMtxF__SetRotateX(Mtx4 *mtx, f32 angle);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void CQuaternion__ToMatrix(CQuaternion *q, Mtx4 *mtx);
void CMtxF__MultiplyAffine(Mtx4 *out, Mtx4 *a, Mtx4 *b);
void CMtxF__Translate(Mtx4 *mtx, f32 x, f32 y, f32 z);
void CMtxF__TransformPoints(Mtx4 *mtx, CVector3 *in, CVector3 *out, s32 count);

/* The eight corners of the mover's collision box (top face first), in world space. */
void Platform_GetCollisionBox(CGameObjectInstance *mover, CVector3 *points, CGameObjectInstance *collider) {
    Mtx4 mtx;
    Mtx4 rot;
    Mtx4 spinMtx;
    CQuaternion q;
    CQuaternion spin;
    CPlatformIntelligence *info;
    CAI *ai;
    f32 halfX;
    f32 halfZ;
    f32 height;
    f32 bottomOff;
    f32 minX, maxX, minZ, maxZ;
    f32 top, bottom;
    f32 x;
    f32 z;
    f32 sinHalf;

    info = &mover->header.m_pIntelligence->m_Platform;
    ai = &mover->m_AI;
    if (info->m_CollisionType == 0) {
        x = mover->header.m_vPos.x;
        minX = x + -10240.0f;
        maxX = x + 10240.0f;
        top = CGameRegion__GetHeight(mover->header.m_pCurrentRegion, x, mover->header.m_vPos.z) + 0.001f;
        z = mover->header.m_vPos.z;
        minZ = z + -10240.0f;
        maxZ = z + 10240.0f;
        bottom = top - 10240.0f;
        points[0].x = minX;
        points[1].x = maxX;
        points[2].x = maxX;
        points[3].x = minX;
        points[4].x = minX;
        points[5].x = maxX;
        points[6].x = maxX;
        points[7].x = minX;
        points[0].y = top;
        points[1].y = top;
        points[2].y = top;
        points[3].y = top;
        points[0].z = maxZ;
        points[1].z = maxZ;
        points[2].z = minZ;
        points[3].z = minZ;
        points[4].z = maxZ;
        points[5].z = maxZ;
        points[6].z = minZ;
        points[7].z = minZ;
    } else {
        switch (info->m_CollisionType) {
        default:
            halfX = info->m_CollisionWidth * 0.5f;
            height = info->m_CollisionHeight;
            bottomOff = 0.0f;
            halfZ = info->m_CollisionLength * 0.5f;
            if (collider != NULL) {
                if (collider->m_dwFlags & 0x300000) {
                    halfX += GameObject_GetCollisionRadius(collider) * 0.5f;
                    bottomOff = -GameObject_GetCollisionHeight(collider);
                    halfZ += GameObject_GetCollisionRadius(collider) * 0.5f;
                } else {
                    halfX += GameObject_GetCollisionRadius(collider);
                    bottomOff = -GameObject_GetCollisionHeight(collider);
                    halfZ += GameObject_GetCollisionRadius(collider);
                }
            }
            break;
        case 1:
            if (collider != NULL) {
                halfZ = info->m_CollisionRadius + GameObject_GetCollisionRadius(collider);
            } else {
                halfZ = info->m_CollisionRadius;
            }
            height = info->m_CollisionHeight;
            bottomOff = 0.0f;
            do { /* block boundary keeps the copy last */
                halfX = halfZ;
            } while (0);
        }
        minX = info->m_CollisionXOffset - halfX;
        maxX = info->m_CollisionXOffset + halfX;
        bottom = info->m_CollisionYOffset + bottomOff;
        top = info->m_CollisionYOffset + height;
        minZ = info->m_CollisionZOffset - halfZ;
        maxZ = info->m_CollisionZOffset + halfZ;
        points[0].x = minX;
        points[1].x = maxX;
        points[2].x = maxX;
        points[3].x = minX;
        points[4].x = minX;
        points[5].x = maxX;
        points[6].x = maxX;
        points[7].x = minX;
        points[0].y = top;
        points[1].y = top;
        points[2].y = top;
        points[3].y = top;
        points[4].y = bottom;
        points[5].y = bottom;
        points[6].y = bottom;
        points[7].y = bottom;
        points[0].z = maxZ;
        points[1].z = maxZ;
        points[2].z = minZ;
        points[3].z = minZ;
        points[4].z = maxZ;
        points[5].z = maxZ;
        points[6].z = minZ;
        points[7].z = minZ;

        CMtxF__SetRotateX(&rot, ai->m_Platform.m_Pitch);
        sinHalf = func_002982F0(mover->m_RotY * 0.5f);
        q.x = 0.0f;
        q.y = sinHalf;
        q.z = 0.0f;
        gQuaternionSin = sinHalf;
        q.w = func_002974C0(mover->m_RotY * 0.5f);
        CQuaternion__Multiply(&spin, &q, &mover->m_qGround);
        CQuaternion__ToMatrix(&spin, &spinMtx);
        CMtxF__MultiplyAffine(&mtx, &rot, &spinMtx);
        CMtxF__Translate(&mtx, mover->header.m_vPos.x, mover->header.m_vPos.y, mover->header.m_vPos.z);
        CMtxF__TransformPoints(&mtx, points, points, 8);
        if (!(info->m_dwFlags & 1)) {
            return;
        }
        bottom = GameObject_GetRegionFloorHeight(mover, 0);
    }
    points[4].y = bottom;
    points[5].y = bottom;
    points[6].y = bottom;
    points[7].y = bottom;
}

#include "structs/collision.h"
#include "structs/region.h"

void Quad_SetBoxBottom(CollisionQuad *quad, CVector3 *points);
s32 Quad_ContainsPointXZ(CollisionQuad *quad, f32 x, f32 z);
f32 Quad_GetHeightAt(CollisionQuad *quad, f32 x, f32 z);

/* Lands the collider on top of the box face of a mover when it is within reach. */
void Platform_LandOnTop(Sweep *ctrl, CGameObjectInstance *collider) {
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
    Platform_GetCollisionBox(mover, points, collider);
    if (!(ctrl->delta.y > 0.0f)) {
        return;
    }
    Quad_SetBoxBottom(&quad, points);
    if (info->m_CollisionType != 0 && !Quad_ContainsPointXZ(&quad, collider->header.m_vPos.x, collider->header.m_vPos.z)) {
        return;
    }
    top = Quad_GetHeightAt(&quad, collider->header.m_vPos.x, collider->header.m_vPos.z);
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

s32 Plane_SegmentCrosses(CollisionQuad *quad, CVector3 *pos, CVector3 *move);
s32 Plane_IntersectSegment(CollisionQuad *quad, CVector3 *pos, CVector3 *move, CVector3 *hit);
s32 Sweep_HitPlane(Sweep *ctrl, CVector3 *normal, CollisionQuad *quad, s32 arg3, s32 arg4, f32 scale, s32 arg6);

/* Collision of the controller's collider against a ground-type (kind 0) mover. */
s32 Platform_CollideGround(Sweep *ctrl, CGameObjectInstance *mover) {
    CGameObjectInstance *collider;
    CVector3 points[8];
    CollisionQuad quad;
    CVector3 hit;
    f32 scale;

    collider = (CGameObjectInstance *)ctrl->obj;
    if (collider->header.m_pCurrentRegion != NULL && mover->header.m_pCurrentRegion->m_nInfo != collider->header.m_pCurrentRegion->m_nInfo) {
        return 0;
    }
    Platform_GetCollisionBox(mover, points, collider);
    Quad_SetBoxBottom(&quad, points);
    if (collider->m_pInstanceBelow == NULL || mover->header.m_vPos.y > collider->m_pInstanceBelow->header.m_vPos.y) {
        collider->m_pInstanceBelow = mover;
    }
    if (Plane_SegmentCrosses(&quad, &collider->header.m_vPos, &ctrl->end)) {
        if (!Plane_IntersectSegment(&quad, &collider->header.m_vPos, &ctrl->end, &hit)) {
            return 0;
        }
        scale = func_002982F0(mover->m_AI.m_Platform.m_Pitch);
        scale *= func_002974C0(mover->m_AI.m_Platform.m_Pitch);
        if (Sweep_HitPlane(ctrl, &quad.normal, &quad, 0, 2, scale, 0)) {
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

#include "globals/collision.h"

typedef void (*FaceFunc)(CollisionQuad *quad, CVector3 *points);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Quad_ContainsPoint Quad_ContainsPoint_f3
#endif

s32 Quad_ContainsPoint(CollisionQuad *quad, f32 x, f32 y, f32 z);

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

void Quad_SetBoxTop(CollisionQuad *quad, CVector3 *points);
void Sphere_Set(Sphere *cyl, CVector3 *center, f32 radius);
s32 Cylinder_ContainsPoint(Sphere *cyl, CVector3 *point);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Sweep_HitCylinder Sweep_HitCylinder_i
#endif

s32 Sweep_HitCylinder(Sweep *ctrl, CVector3 *center, f32 radius, s32 arg3, f32 top, f32 bottom, s32 arg6, s32 arg7, s32 arg8, s32 arg9);

/* Collision of the controller's collider against a cylinder-type (kind 1) mover. */
s32 Platform_CollideCylinder(Sweep *ctrl, CGameObjectInstance *mover) {
    CGameObjectInstance *collider;
    CIntelligence *def;
    CPlatformIntelligence *info;
    CVector3 points[8];
    CollisionQuad quad;
    Sphere cyl;
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

/* Height at (x, z) of the face built from points 0-3 of the object's box. */
f32 Platform_GetTopHeight(void *object, f32 x, f32 z) {
    CVector3 points[8];
    CollisionQuad quad;

    Platform_GetCollisionBox(object, points, 0);
    Quad_SetBoxBottom(&quad, points);
    return Quad_GetHeightAt(&quad, x, z);
}

void Platform_Collide(Sweep *ctrl, CGameObjectInstance *actor) {
    CPlatformIntelligence *info;
    CCollisionInfo *def;
    s32 ok;

    info = &actor->header.m_pIntelligence->m_Platform;
    def = ctrl->def;
    if (def != &gCollisionInfoPlatformRider && def->m_InstanceBehavior != 0) {
        switch (info->m_CollisionType) {
            case 0:
                ok = Platform_CollideGround(ctrl, actor);
                break;
            case 1:
                ok = Platform_CollideCylinder(ctrl, actor);
                break;
            case 2:
                ok = Platform_CollideBox(ctrl, actor);
                break;
            default:
                ok = 0;
                break;
        }
        if (ok != 0) {
            ctrl->hit.kindOut = NULL;
            ctrl->hit.kind = 0;
            ctrl->hit.data = actor;
            ctrl->hit.posOut = &gCollisionResultsP->m_vShapePos;
            ctrl->hit.regionOut = &gCollisionResultsP->m_pShapeRegion;
            ctrl->hit.dataOut = (void **)&gCollisionResultsP->m_pShape;
        }
    }
}

s32 Platform_HitFace(void *arg0, CGameObjectInstance *mover, CGameObjectInstance *collider, char *arg3) {
    f32 scale;

    scale = func_002982F0(mover->m_AI.m_Platform.m_Pitch);
    scale *= func_002974C0(mover->m_AI.m_Platform.m_Pitch);
    if (Sweep_HitPlane(arg0, arg3 + 0x30, arg3, 0, 2, scale, 0) != 0) {
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
