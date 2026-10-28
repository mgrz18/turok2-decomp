#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Actor {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ void *region;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ Vec3 velocity;
} Actor;

typedef struct CollisionInfo {
    /* 0x00 */ s32 flags;
} CollisionInfo;

extern CollisionInfo D_800F1BE0;

s32 func_00267F64(Actor *actor);
s32 func_00267F10(Actor *actor);
f32 func_0026BE60(void *region, f32 x, f32 z);
void func_00239F00(Actor *actor, Vec3 pos, CollisionInfo *info);

/* Slides an actor to (x, z) with collision, keeping its height above the floor. */
void func_002694A8(Actor *actor, f32 x, f32 z) {
    Vec3 savedVelocity;
    Vec3 to;
    s32 noA;
    s32 noB;
    s32 oldFlags;
    f32 height;

    noA = func_00267F64(actor) == 0;
    noB = func_00267F10(actor) == 0;
    height = actor->pos.y - func_0026BE60(actor->region, actor->pos.x, actor->pos.z);
    savedVelocity = actor->velocity;
    actor->velocity.x = actor->velocity.y = actor->velocity.z = 0.0f;
    to.x = x;
    to.y = actor->pos.y;
    to.z = z;
    oldFlags = D_800F1BE0.flags;
    if (noA) {
        D_800F1BE0.flags = oldFlags & ~0x40000;
    } else {
        D_800F1BE0.flags = oldFlags | 0x40000;
    }
    if (noB) {
        D_800F1BE0.flags &= ~0x30000;
    } else {
        D_800F1BE0.flags |= 0x30000;
    }
    func_00239F00(actor, to, &D_800F1BE0);
    D_800F1BE0.flags = oldFlags;
    actor->velocity = savedVelocity;
    actor->pos.y = func_0026BE60(actor->region, actor->pos.x, actor->pos.z) + height;
}
