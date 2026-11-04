#include "common.h"
#include "structs/vector.h"
#include "structs/collision.h"

typedef struct DefInfo {
    char pad0[4];
    u16 kind;          /* 0x04 */
} DefInfo;

typedef struct Def {
    char pad0[0x14];
    DefInfo info;      /* 0x14 */
} Def;

typedef struct Mover {
    char pad0[0x10];
    u16 *group;        /* 0x10 */
    Def *def;          /* 0x14 */
    char pad18[0x140 - 0x18];
    s32 flags;         /* 0x140 */
} Mover;

typedef struct Collider {
    u8 kind;           /* 0x00 */
    char pad1[3];
    f32 x;             /* 0x04 */
    f32 y;             /* 0x08 */
    f32 z;             /* 0x0C */
    u16 *group;        /* 0x10 */
    char pad14[0x1C - 0x14];
    f32 unk1C;         /* 0x1C */
    char pad20[0x30 - 0x20];
    Mover *mover;      /* 0x30 */
    char pad34[0xD4 - 0x34];
    s32 flags;         /* 0xD4 */
} Collider;

void func_00241960(Mover *mover, CVector3 *points, Collider *collider);
void func_0023C9CC(CollisionQuad *quad, CVector3 *points);
s32 func_0023C730(CollisionQuad *quad, f32 x, f32 z);
f32 func_0023C8A0(CollisionQuad *quad, f32 x, f32 z);

/* Lands the collider on top of the box face of a mover when it is within reach. */
void func_00241CB0(Sweep *ctrl, Collider *collider) {
    Mover *mover;
    DefInfo *info;
    CVector3 points[8];
    CollisionQuad quad;
    f32 top;
    f32 y;
    f32 reach;

    mover = (Mover *)ctrl->obj;
    info = &mover->def->info;
    if (info->kind == 0 && collider->group != NULL && *mover->group != *collider->group) {
        return;
    }
    func_00241960(mover, points, collider);
    if (!(ctrl->delta.y > 0.0f)) {
        return;
    }
    func_0023C9CC(&quad, points);
    if (info->kind != 0 && !func_0023C730(&quad, collider->x, collider->z)) {
        return;
    }
    top = func_0023C8A0(&quad, collider->x, collider->z);
    y = collider->y;
    reach = top + ctrl->delta.y;
    if (!(top <= y) && info->kind != 0) {
        return;
    }
    if (y <= reach) {
        collider->mover = mover;
        collider->y = top + 0.4096f;
        if (collider->unk1C < 0.0f) {
            collider->unk1C = 0.0f;
        }
        mover->flags |= 0x10000;
        if (collider->kind == 1 && (collider->flags & 0x300000)) {
            mover->flags |= 0x20000;
        }
    }
}
