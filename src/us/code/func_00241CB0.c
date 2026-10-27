#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Quad {
    Vec3 corner[4]; /* 0x00 */
    Vec3 normal;    /* 0x30 */
} Quad;

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

typedef struct Controller {
    Mover *mover;      /* 0x00 */
    char pad4[0x5C - 0x4];
    f32 height;        /* 0x5C */
} Controller;

void func_00241960(Mover *mover, Vec3 *points, Collider *collider);
void func_0023C9CC(Quad *quad, Vec3 *points);
s32 func_0023C730(Quad *quad, f32 x, f32 z);
f32 func_0023C8A0(Quad *quad, f32 x, f32 z);

/* Lands the collider on top of the box face of a mover when it is within reach. */
void func_00241CB0(Controller *ctrl, Collider *collider) {
    Mover *mover;
    DefInfo *info;
    Vec3 points[8];
    Quad quad;
    f32 top;
    f32 y;
    f32 reach;

    mover = ctrl->mover;
    info = &mover->def->info;
    if (info->kind == 0 && collider->group != NULL && *mover->group != *collider->group) {
        return;
    }
    func_00241960(mover, points, collider);
    if (!(ctrl->height > 0.0f)) {
        return;
    }
    func_0023C9CC(&quad, points);
    if (info->kind != 0 && !func_0023C730(&quad, collider->x, collider->z)) {
        return;
    }
    top = func_0023C8A0(&quad, collider->x, collider->z);
    y = collider->y;
    reach = top + ctrl->height;
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
