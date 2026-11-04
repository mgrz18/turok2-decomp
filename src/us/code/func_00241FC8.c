#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

typedef struct Mover {
    char pad0[4];
    CVector3 pos;          /* 0x04 */
    u16 *group;        /* 0x10 */
    char pad14[0x140 - 0x14];
    s32 flags;         /* 0x140 */
    char pad144[0x25C - 0x144];
    f32 angle;         /* 0x25C */
} Mover;

typedef struct Collider {
    u8 kind;           /* 0x00 */
    char pad1[3];
    CVector3 pos;          /* 0x04 */
    u16 *group;        /* 0x10 */
    char pad14[0x1C - 0x14];
    f32 unk1C;         /* 0x1C */
    char pad20[0x30 - 0x20];
    Mover *mover;      /* 0x30 */
    char pad34[0xD4 - 0x34];
    s32 flags;         /* 0xD4 */
} Collider;

typedef void (*FaceFunc)(CollisionQuad *quad, CVector3 *points);

/* The side faces of a box: func_0023CDC8, func_0023CC30, func_0023CCFC, func_0023CA98, func_0023CB64. */
extern FaceFunc D_800AD930[];

void func_00241960(Mover *mover, CVector3 *points, Collider *collider);
void func_0023C9CC(CollisionQuad *quad, CVector3 *points);
s32 func_0023C730(CollisionQuad *quad, f32 x, f32 z);
s32 func_0023C2B0(CollisionQuad *quad, CVector3 *pos, CVector3 *move);
s32 func_0023C478(CollisionQuad *quad, CVector3 *pos, CVector3 *move, CVector3 *hit);
s32 func_0023C594(CollisionQuad *quad, f32 x, f32 y, f32 z);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
s32 func_0023AB18(Sweep *ctrl, CVector3 *normal, CollisionQuad *quad, s32 arg3, s32 arg4, f32 scale, s32 arg6);

/* func_002426F4, inlined. */
static __inline__ s32 LandOn(Sweep *ctrl, Mover *mover, Collider *collider, CollisionQuad *quad) {
    f32 scale;

    scale = func_002982F0(mover->angle);
    scale *= func_002974C0(mover->angle);
    if (func_0023AB18(ctrl, &quad->normal, quad, 0, 2, scale, 0) != 0) {
        mover->flags |= 0x10000;
        if (collider->kind == 1 && (collider->flags & 0x300000)) {
            mover->flags |= 0x20000;
        }
        collider->mover = mover;
        if (collider->unk1C < 0.0f) {
            collider->unk1C = 0.0f;
        }
        return 1;
    }
    return 0;
}

/* Collision of the controller's collider against a box-type (kind 2) mover. */
s32 func_00241FC8(Sweep *ctrl, Mover *mover) {
    Collider *collider;
    CVector3 points[8];
    CollisionQuad quad;
    CVector3 hit;
    s32 result;
    s32 i;

    collider = (Collider *)ctrl->obj;
    result = 0;
    func_00241960(mover, points, collider);
    i = 0;
    while (D_800AD930[i] != NULL) {
        D_800AD930[i++](&quad, points);
        if (func_0023C2B0(&quad, &collider->pos, &ctrl->end)
            && func_0023C478(&quad, &collider->pos, &ctrl->end, &hit)
            && func_0023C594(&quad, hit.x, hit.y, hit.z)) {
            result |= func_0023AB18(ctrl, &quad.normal, &quad, 1, (i == 0) ? 2 : ctrl->def->m_InstanceBehavior, 0.0f, 0);
        }
    }
    func_0023C9CC(&quad, points);
    if (func_0023C730(&quad, ctrl->end.x, ctrl->end.z)) {
        if (collider->mover == NULL || mover->pos.y > collider->mover->pos.y) {
            collider->mover = mover;
        }
    }
    if (func_0023C2B0(&quad, &collider->pos, &ctrl->end)) {
        if (!func_0023C478(&quad, &collider->pos, &ctrl->end, &hit)) {
            return result;
        }
        if (!func_0023C594(&quad, hit.x, hit.y, hit.z)) {
            return result;
        }
        result |= LandOn(ctrl, mover, collider, &quad);
    }
    return result;
}
