#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/collision.h"
#include "structs/rect.h"
#include "globals/collision.h"

f32 GameObject_GetCollisionHeight(CGameObjectInstance *inst);
f32 GameObject_GetCollisionRadius(CGameObjectInstance *inst);
f32 GameObject_GetCollisionYOffset(CGameObjectInstance *inst);
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
s32 Sweep_HitPlane(Sweep *sweep, CVector3 *normal, CVector3 *hit, s32 arg3, s32 arg4, f32 arg5, s32 arg6);
s32 Sweep_HitCylinder(Sweep *sweep, CVector3 *centre, f32 radius, s32 checkHeight, f32 maxY, f32 minY,
                  s32 frontOnly, s32 arg7, f32 arg8, s32 fromInside);
s32 Sweep_IsSpecialObject(CObjectHead *other, CObjectHead *inst);
void SweepHit_Clear(f32 *hit);
void Platform_LandOnTop(Sweep *sweep, CObjectHead *other);
void Platform_Collide(Sweep *sweep, CObjectHead *other);

/* Sweep an instance's volume (box) against another instance's cylinder:
 * its top and bottom discs when the box asks for it, then its side. */
void Sweep_TestInstance(Sweep *sweep, SweepBox *box, CObjectHead *other) {
    CRect rect;
    CVector3 hit;
    CVector3 normal;
    f32 x;
    f32 z;
    f32 radius;
    f32 reach;
    f32 bottom;
    f32 top;
    f32 t;
    f32 dx;
    f32 dz;
    CObjectHead *inst;
    CCollisionInfo *def;
    CCollisionResults *target;
    s32 result;
    s32 special;

    x = other->header.m_vPos.x;
    z = other->header.m_vPos.z;
    radius = GameObject_GetCollisionRadius((CGameObjectInstance *) other);
    rect.minX = x - radius;
    rect.maxX = x + radius;
    rect.minZ = z - radius;
    rect.maxZ = z + radius;
    if (!(rect.minX < box->maxX) || !(box->minX < rect.maxX) || !(rect.minZ < box->maxZ) ||
        !(box->minZ < rect.maxZ)) {
        return;
    }
    inst = (CObjectHead *)sweep->inst;
    def = sweep->def;
    special = 0;
    if (inst->header.m_Type == 1 && inst->header.m_pIntelligence->m_Type == 2) {
        if (other->header.m_Type == inst->header.m_Type) {
            Platform_LandOnTop(sweep, other);
        }
        return;
    }
    if (other->header.m_Type == 1 && other->header.m_pIntelligence->m_Type == 2) {
        if (inst->header.m_Type == other->header.m_Type) {
            Platform_Collide(sweep, other);
        }
        return;
    }
    if (other->header.m_Type == 3) {
        if (!Sweep_IsSpecialObject(other, inst)) {
            return;
        }
        special = 1;
    }
    bottom = other->header.m_vPos.y + GameObject_GetCollisionYOffset((CGameObjectInstance *) other);
    top = GameObject_GetCollisionHeight((CGameObjectInstance *) other) + bottom;
    if (box->flag) {
        normal.x = normal.z = 0.0f;
    }
    result = 0;
    reach = box->radius + radius;
    if (box->flag && sweep->delta.y != 0.0f) {
        if (sweep->lastKind == 2 && sweep->lastRadius == reach && sweep->lastContact.x == other->header.m_vPos.x &&
            sweep->lastContact.y == other->header.m_vPos.y && sweep->lastContact.z == other->header.m_vPos.z) {
            return;
        }
        if (box->startTop <= bottom && bottom < box->endTop) {
            t = (bottom - box->startTop) / sweep->delta.y;
            if (t < sweep->hit.fraction) {
                CVector3__Lerp(&hit, t, &inst->header.m_vPos, &sweep->end);
                dx = x - hit.x;
                dz = z - hit.z;
                if (dx * dx + dz * dz < reach * reach) {
                    normal.y = -1.0f;
                    result |= Sweep_HitPlane(sweep, &normal, &hit, 0, 7, 0.0f, 1);
                }
            }
        }
        if (top <= box->startBottom && box->endBottom < top) {
            t = (top - box->startBottom) / sweep->delta.y;
            if (t < sweep->hit.fraction) {
                CVector3__Lerp(&hit, t, &inst->header.m_vPos, &sweep->end);
                dx = x - hit.x;
                dz = z - hit.z;
                if (dx * dx + dz * dz < reach * reach) {
                    normal.y = 1.0f;
                    if (other->header.m_pIntelligence->m_dwFlags & 0x80) {
                        if (Sweep_HitPlane(sweep, &normal, &hit, 0, 2, 0.0f, 1)) {
                            result |= 1;
                            if (inst->m_vVelocity.y < 0.0f) {
                                inst->m_vVelocity.y = 0.0f;
                                inst->m_pInstanceBelow = (CGameObjectInstance *)other;
                            }
                        }
                    } else {
                        result |= Sweep_HitPlane(sweep, &normal, &hit, 0, 7, 0.0f, 1);
                    }
                }
            }
        }
    }
    result |= Sweep_HitCylinder(sweep, &other->header.m_vPos, reach, 1, top - box->bottom, bottom - box->top, 1,
                            special ? 7 : def->m_InstanceBehavior, 0.0f, 1);
    if (result) {
        target = gCollisionResultsP;
        if (target->m_pInstance) {
            SweepHit_Clear(&sweep->hit.fraction);
        } else {
            sweep->hit.posOut = &target->m_vInstancePos;
            sweep->hit.regionOut = &target->m_pInstanceRegion;
            sweep->hit.kindOut = &target->m_nInstanceNode;
            sweep->hit.kind = -1;
            sweep->hit.dataOut = (void **)&target->m_pInstance;
            sweep->hit.data = other;
        }
        if (box->flag) {
            sweep->kind = 2;
            sweep->contact = other->header.m_vPos;
            sweep->radius = reach;
        }
    }
}
