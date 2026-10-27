#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Quad {
    Vec3 corner[4]; /* 0x00 */
    Vec3 normal;    /* 0x30 */
} Quad;

typedef struct CylInfo {
    s32 flags;         /* 0x00 */
    char pad4[4];
    f32 radius;        /* 0x08 */
    char padC[4];
    f32 height;        /* 0x10 */
} CylInfo;

typedef struct Def {
    char pad0[0x14];
    CylInfo info;      /* 0x14 */
} Def;

typedef struct Cylinder {
    f32 v[6];
} Cylinder;

typedef struct Mover {
    char pad0[4];
    Vec3 pos;          /* 0x04 */
    u16 *group;        /* 0x10 */
    Def *def;          /* 0x14 */
    char pad18[0x140 - 0x18];
    s32 flags;         /* 0x140 */
    char pad144[0x25C - 0x144];
    f32 angle;         /* 0x25C */
} Mover;

typedef struct Collider {
    u8 kind;           /* 0x00 */
    char pad1[3];
    Vec3 pos;          /* 0x04 */
    u16 *group;        /* 0x10 */
    char pad14[0x1C - 0x14];
    f32 unk1C;         /* 0x1C */
    char pad20[0x30 - 0x20];
    Mover *mover;      /* 0x30 */
    char pad34[0xD4 - 0x34];
    s32 flags;         /* 0xD4 */
} Collider;

typedef struct Target {
    char pad0[4];
    s8 active;         /* 0x04 */
} Target;

typedef struct Controller {
    Collider *collider; /* 0x00 */
    Target *target;     /* 0x04 */
    char pad8[0x4C - 0x8];
    Vec3 move;          /* 0x4C */
} Controller;

void func_00241960(Mover *mover, Vec3 *points, Collider *collider);
void func_0023C9CC(Quad *quad, Vec3 *points);
void func_0023CDC8(Quad *quad, Vec3 *points);
s32 func_0023C730(Quad *quad, f32 x, f32 z);
s32 func_0023C2B0(Quad *quad, Vec3 *pos, Vec3 *move);
s32 func_0023C478(Quad *quad, Vec3 *pos, Vec3 *move, Vec3 *hit);
f32 func_002672C8(Collider *collider);
f32 func_002671B4(Collider *collider);
void func_0023CE94(Cylinder *cyl, Vec3 *center, f32 radius);
s32 func_0023CEC0(Cylinder *cyl, Vec3 *point);
s32 func_0023B38C(Controller *ctrl, Vec3 *center, f32 radius, s32 arg3, f32 top, f32 bottom, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
s32 func_0023AB18(Controller *ctrl, Vec3 *normal, Quad *quad, s32 arg3, s32 arg4, f32 scale, s32 arg6);

/* func_002426F4, inlined. */
static __inline__ s32 LandOn(Controller *ctrl, Mover *mover, Collider *collider, Quad *quad) {
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

/* Collision of the controller's collider against a cylinder-type (kind 1) mover. */
s32 func_00242284(Controller *ctrl, Mover *mover) {
    Collider *collider;
    Def *def;
    CylInfo *info;
    Vec3 points[8];
    Quad quad;
    Cylinder cyl;
    Vec3 hit;
    s32 result;
    f32 radius;
    f32 lift;
    f32 y;

    collider = ctrl->collider;
    def = mover->def;
    func_00241960(mover, points, collider);
    info = &def->info;
    func_0023CE94(&cyl, &mover->pos, info->radius + func_002672C8(collider));
    if (def->info.flags & 1) {
        result = func_0023B38C(ctrl, &mover->pos, info->radius + func_002672C8(collider), 1,
                               mover->pos.y + info->height, collider->pos.y, 1, ctrl->target->active, 0, 0);
    } else {
        radius = info->radius + func_002672C8(collider);
        lift = func_002671B4(collider);
        y = mover->pos.y;
        result = func_0023B38C(ctrl, &mover->pos, radius, 1,
                               y + info->height, y - lift, 1, ctrl->target->active, 0, 0);
    }
    func_0023C9CC(&quad, points);
    if (func_0023C730(&quad, ctrl->move.x, ctrl->move.z)) {
        if (collider->mover == NULL || mover->pos.y > collider->mover->pos.y) {
            collider->mover = mover;
        }
    }
    if (func_0023C2B0(&quad, &collider->pos, &ctrl->move)
        && func_0023C478(&quad, &collider->pos, &ctrl->move, &hit)
        && func_0023CEC0(&cyl, &hit)) {
        result |= LandOn(ctrl, mover, collider, &quad);
    }
    func_0023CDC8(&quad, points);
    if (func_0023C2B0(&quad, &collider->pos, &ctrl->move)
        && func_0023C478(&quad, &collider->pos, &ctrl->move, &hit)
        && func_0023CEC0(&cyl, &hit)) {
        result |= func_0023AB18(ctrl, &quad.normal, &quad, 1, 2, 0.0f, 0);
    }
    return result;
}
