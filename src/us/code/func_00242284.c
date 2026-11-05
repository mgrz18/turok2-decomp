#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

typedef struct Cylinder {
    f32 v[6];
} Cylinder;

void func_00241960(CGameObjectInstance *mover, CVector3 *points, CGameObjectInstance *collider);
void func_0023C9CC(CollisionQuad *quad, CVector3 *points);
void func_0023CDC8(CollisionQuad *quad, CVector3 *points);
s32 func_0023C730(CollisionQuad *quad, f32 x, f32 z);
s32 func_0023C2B0(CollisionQuad *quad, CVector3 *pos, CVector3 *move);
s32 func_0023C478(CollisionQuad *quad, CVector3 *pos, CVector3 *move, CVector3 *hit);
f32 func_002672C8(CGameObjectInstance *collider);
f32 func_002671B4(CGameObjectInstance *collider);
void func_0023CE94(Cylinder *cyl, CVector3 *center, f32 radius);
s32 func_0023CEC0(Cylinder *cyl, CVector3 *point);
s32 func_0023B38C(Sweep *ctrl, CVector3 *center, f32 radius, s32 arg3, f32 top, f32 bottom, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
s32 func_0023AB18(Sweep *ctrl, CVector3 *normal, CollisionQuad *quad, s32 arg3, s32 arg4, f32 scale, s32 arg6);

/* func_002426F4, inlined. */
static __inline__ s32 LandOn(Sweep *ctrl, CGameObjectInstance *mover, CGameObjectInstance *collider, CollisionQuad *quad) {
    f32 scale;

    scale = func_002982F0(mover->m_AI.m_Platform.m_Pitch);
    scale *= func_002974C0(mover->m_AI.m_Platform.m_Pitch);
    if (func_0023AB18(ctrl, &quad->normal, quad, 0, 2, scale, 0) != 0) {
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
s32 func_00242284(Sweep *ctrl, CGameObjectInstance *mover) {
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
    func_00241960(mover, points, collider);
    info = &def->m_Platform;
    func_0023CE94(&cyl, &mover->header.m_vPos, info->m_CollisionRadius + func_002672C8(collider));
    if (def->m_Platform.m_dwFlags & 1) {
        result = func_0023B38C(ctrl, &mover->header.m_vPos, info->m_CollisionRadius + func_002672C8(collider), 1,
                               mover->header.m_vPos.y + info->m_CollisionHeight, collider->header.m_vPos.y, 1, ctrl->def->m_InstanceBehavior, 0, 0);
    } else {
        radius = info->m_CollisionRadius + func_002672C8(collider);
        lift = func_002671B4(collider);
        y = mover->header.m_vPos.y;
        result = func_0023B38C(ctrl, &mover->header.m_vPos, radius, 1,
                               y + info->m_CollisionHeight, y - lift, 1, ctrl->def->m_InstanceBehavior, 0, 0);
    }
    func_0023C9CC(&quad, points);
    if (func_0023C730(&quad, ctrl->end.x, ctrl->end.z)) {
        if (collider->m_pInstanceBelow == NULL || mover->header.m_vPos.y > collider->m_pInstanceBelow->header.m_vPos.y) {
            collider->m_pInstanceBelow = mover;
        }
    }
    if (func_0023C2B0(&quad, &collider->header.m_vPos, &ctrl->end)
        && func_0023C478(&quad, &collider->header.m_vPos, &ctrl->end, &hit)
        && func_0023CEC0(&cyl, &hit)) {
        result |= LandOn(ctrl, mover, collider, &quad);
    }
    func_0023CDC8(&quad, points);
    if (func_0023C2B0(&quad, &collider->header.m_vPos, &ctrl->end)
        && func_0023C478(&quad, &collider->header.m_vPos, &ctrl->end, &hit)
        && func_0023CEC0(&cyl, &hit)) {
        result |= func_0023AB18(ctrl, &quad.normal, &quad, 1, 2, 0.0f, 0);
    }
    return result;
}
