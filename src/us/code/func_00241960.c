#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Quat {
    f32 x, y, z, w;
} Quat;

typedef f32 Mtx4[4][4];

typedef struct BoxInfo {
    s32 flags;         /* 0x00 */
    u16 kind;          /* 0x04: 0 ground, 1 cylinder, 2 box */
    char pad6[2];
    f32 radius;        /* 0x08 */
    f32 width;         /* 0x0C */
    f32 height;        /* 0x10 */
    f32 depth;         /* 0x14 */
    Vec3 center;       /* 0x18 */
} BoxInfo;

typedef struct Def {
    char pad0[0x14];
    BoxInfo info;      /* 0x14 */
} Def;

typedef struct AI {
    char pad0[0x11C];
    f32 angle;         /* 0x11C */
} AI;

typedef struct Mover {
    char pad0[4];
    Vec3 pos;          /* 0x04 */
    void *region;      /* 0x10 */
    Def *def;          /* 0x14 */
    char pad18[0x40 - 0x18];
    Quat rotation;     /* 0x40 */
    f32 spin;          /* 0x50 */
    char pad54[0x140 - 0x54];
    AI ai;             /* 0x140 */
} Mover;

typedef struct Collider {
    char pad0[0xD4];
    s32 flags;         /* 0xD4 */
} Collider;

extern f32 D_800DD78C;

f32 func_0026BE60(void *region, f32 x, f32 z);
f32 func_002672C8(Collider *collider);
f32 func_002671B4(Collider *collider);
f32 func_0026872C(Mover *mover, s32 arg1);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void func_00210518(Mtx4 *mtx, f32 angle);
void func_00210F68(Quat *out, Quat *a, Quat *b);
void func_00211114(Quat *q, Mtx4 *mtx);
void func_0020C618(Mtx4 *out, Mtx4 *a, Mtx4 *b);
void func_00210318(Mtx4 *mtx, f32 x, f32 y, f32 z);
void func_0020F908(Mtx4 *mtx, Vec3 *in, Vec3 *out, s32 count);

/* The eight corners of the mover's collision box (top face first), in world space. */
void func_00241960(Mover *mover, Vec3 *points, Collider *collider) {
    Mtx4 mtx;
    Mtx4 rot;
    Mtx4 spinMtx;
    Quat q;
    Quat spin;
    BoxInfo *info;
    AI *ai;
    f32 halfX;
    f32 halfZ;
    f32 height;
    f32 bottomOff;
    f32 minX, maxX, minZ, maxZ;
    f32 top, bottom;
    f32 x;
    f32 z;
    f32 sinHalf;

    info = &mover->def->info;
    ai = &mover->ai;
    if (info->kind == 0) {
        x = mover->pos.x;
        minX = x + -10240.0f;
        maxX = x + 10240.0f;
        top = func_0026BE60(mover->region, x, mover->pos.z) + 0.001f;
        z = mover->pos.z;
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
        switch (info->kind) {
        default:
            halfX = info->width * 0.5f;
            height = info->height;
            bottomOff = 0.0f;
            halfZ = info->depth * 0.5f;
            if (collider != NULL) {
                if (collider->flags & 0x300000) {
                    halfX += func_002672C8(collider) * 0.5f;
                    bottomOff = -func_002671B4(collider);
                    halfZ += func_002672C8(collider) * 0.5f;
                } else {
                    halfX += func_002672C8(collider);
                    bottomOff = -func_002671B4(collider);
                    halfZ += func_002672C8(collider);
                }
            }
            break;
        case 1:
            if (collider != NULL) {
                halfZ = info->radius + func_002672C8(collider);
            } else {
                halfZ = info->radius;
            }
            height = info->height;
            bottomOff = 0.0f;
            do { /* block boundary keeps the copy last */
                halfX = halfZ;
            } while (0);
        }
        minX = info->center.x - halfX;
        maxX = info->center.x + halfX;
        bottom = info->center.y + bottomOff;
        top = info->center.y + height;
        minZ = info->center.z - halfZ;
        maxZ = info->center.z + halfZ;
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

        func_00210518(&rot, ai->angle);
        sinHalf = func_002982F0(mover->spin * 0.5f);
        q.x = 0.0f;
        q.y = sinHalf;
        q.z = 0.0f;
        D_800DD78C = sinHalf;
        q.w = func_002974C0(mover->spin * 0.5f);
        func_00210F68(&spin, &q, &mover->rotation);
        func_00211114(&spin, &spinMtx);
        func_0020C618(&mtx, &rot, &spinMtx);
        func_00210318(&mtx, mover->pos.x, mover->pos.y, mover->pos.z);
        func_0020F908(&mtx, points, points, 8);
        if (!(info->flags & 1)) {
            return;
        }
        bottom = func_0026872C(mover, 0);
    }
    points[4].y = bottom;
    points[5].y = bottom;
    points[6].y = bottom;
    points[7].y = bottom;
}
