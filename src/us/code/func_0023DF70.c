#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "structs/rect.h"
#include "globals/collision.h"

typedef struct HitDef {
    /* 0x00 */ s32 kind;
    /* 0x04 */ s32 flags;
} HitDef;

/* An instance that can be hit (or the instance sweeping). */
#ifdef TARGET_PORT
typedef struct HitInst {
/* the view at the host offsets of CObjectHead (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_HitInst(m) __builtin_offsetof(CObjectHead, m)
    union {
        struct { u8 _hp_type[T2_HOFF_HitInst(header.m_Type)]; u8 type; }; /* N64 0x0 */
        struct { u8 _hp_pos[T2_HOFF_HitInst(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_def[T2_HOFF_HitInst(header.m_pIntelligence)]; HitDef *def; }; /* N64 0x14 */
        struct { u8 _hp_fallSpeed[T2_HOFF_HitInst(m_vVelocity.y)]; f32 fallSpeed; }; /* N64 0x1C */
        struct { u8 _hp_standingOn[T2_HOFF_HitInst(m_pInstanceBelow)]; struct HitInst *standingOn; }; /* N64 0x30 */
    };
} HitInst;
#else
typedef struct HitInst {
    /* 0x00 */ u8 type;
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ HitDef *def;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ f32 fallSpeed;
    /* 0x20 */ u8 pad20[0x10];
    /* 0x30 */ struct HitInst *standingOn;
} HitInst;
#endif


f32 GameObject_GetCollisionHeight(HitInst *inst);
f32 GameObject_GetCollisionRadius(HitInst *inst);
f32 func_0026841C(HitInst *inst);
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
s32 Sweep_HitPlane(Sweep *sweep, CVector3 *normal, CVector3 *hit, s32 arg3, s32 arg4, f32 arg5, s32 arg6);
s32 Sweep_HitCylinder(Sweep *sweep, CVector3 *centre, f32 radius, s32 checkHeight, f32 maxY, f32 minY,
                  s32 frontOnly, s32 arg7, f32 arg8, s32 fromInside);
s32 Sweep_IsSpecialObject(HitInst *other, HitInst *inst);
void SweepHit_Clear(f32 *hit);
void Platform_LandOnTop(Sweep *sweep, HitInst *other);
void Platform_Collide(Sweep *sweep, HitInst *other);

/* Sweep an instance's volume (box) against another instance's cylinder:
 * its top and bottom discs when the box asks for it, then its side. */
void Sweep_TestInstance(Sweep *sweep, SweepBox *box, HitInst *other) {
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
    HitInst *inst;
    CCollisionInfo *def;
    CCollisionResults *target;
    s32 result;
    s32 special;

    x = other->pos.x;
    z = other->pos.z;
    radius = GameObject_GetCollisionRadius(other);
    rect.minX = x - radius;
    rect.maxX = x + radius;
    rect.minZ = z - radius;
    rect.maxZ = z + radius;
    if (!(rect.minX < box->maxX) || !(box->minX < rect.maxX) || !(rect.minZ < box->maxZ) ||
        !(box->minZ < rect.maxZ)) {
        return;
    }
    inst = (HitInst *)sweep->inst;
    def = sweep->def;
    special = 0;
    if (inst->type == 1 && inst->def->kind == 2) {
        if (other->type == inst->type) {
            Platform_LandOnTop(sweep, other);
        }
        return;
    }
    if (other->type == 1 && other->def->kind == 2) {
        if (inst->type == other->type) {
            Platform_Collide(sweep, other);
        }
        return;
    }
    if (other->type == 3) {
        if (!Sweep_IsSpecialObject(other, inst)) {
            return;
        }
        special = 1;
    }
    bottom = other->pos.y + func_0026841C(other);
    top = GameObject_GetCollisionHeight(other) + bottom;
    if (box->flag) {
        normal.x = normal.z = 0.0f;
    }
    result = 0;
    reach = box->radius + radius;
    if (box->flag && sweep->delta.y != 0.0f) {
        if (sweep->lastKind == 2 && sweep->lastRadius == reach && sweep->lastContact.x == other->pos.x &&
            sweep->lastContact.y == other->pos.y && sweep->lastContact.z == other->pos.z) {
            return;
        }
        if (box->startTop <= bottom && bottom < box->endTop) {
            t = (bottom - box->startTop) / sweep->delta.y;
            if (t < sweep->hit.fraction) {
                CVector3__Lerp(&hit, t, &inst->pos, &sweep->end);
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
                CVector3__Lerp(&hit, t, &inst->pos, &sweep->end);
                dx = x - hit.x;
                dz = z - hit.z;
                if (dx * dx + dz * dz < reach * reach) {
                    normal.y = 1.0f;
                    if (other->def->flags & 0x80) {
                        if (Sweep_HitPlane(sweep, &normal, &hit, 0, 2, 0.0f, 1)) {
                            result |= 1;
                            if (inst->fallSpeed < 0.0f) {
                                inst->fallSpeed = 0.0f;
                                inst->standingOn = other;
                            }
                        }
                    } else {
                        result |= Sweep_HitPlane(sweep, &normal, &hit, 0, 7, 0.0f, 1);
                    }
                }
            }
        }
    }
    result |= Sweep_HitCylinder(sweep, &other->pos, reach, 1, top - box->bottom, bottom - box->top, 1,
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
            sweep->contact = other->pos;
            sweep->radius = reach;
        }
    }
}
