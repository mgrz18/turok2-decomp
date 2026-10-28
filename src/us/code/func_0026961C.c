#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct ActorMotion {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ void *region;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ Vec3 velocity;
    /* 0x24 */ u8 pad24[0x34 - 0x24];
} ActorMotion; /* size 0x34 */

typedef struct Actor {
    /* 0x00 */ ActorMotion motion;
    /* 0x34 */ u8 pad34[0xD4 - 0x34];
    /* 0xD4 */ s32 flags;
} Actor;

typedef struct CollisionInfo {
    /* 0x00 */ s32 flags;
} CollisionInfo;

extern CollisionInfo D_800F1B00;
extern CollisionInfo D_800F1B60;
extern CollisionInfo D_800F1B80;
extern CollisionInfo D_800F1BE0;

s32 func_00267EE8(Actor *actor);
s32 func_00267F64(Actor *actor);
s32 func_00267FA8(Actor *actor);
s32 func_00239F00(Actor *actor, Vec3 pos, CollisionInfo *info);

/* Tests moving an actor to pos with the collision settings that suit it, leaving it where it was; returns the collision result. */
s32 func_0026961C(Actor *actor, Vec3 pos, CollisionInfo **outInfo) {
    ActorMotion saved;
    CollisionInfo *info;
    s32 alt;
    s32 hit;

    if (actor->motion.kind == 1 && (actor->flags & 0x300000)) {
        info = &D_800F1BE0;
    } else {
        alt = func_00267EE8(actor);
        if (!func_00267F64(actor)) {
            info = alt ? &D_800F1B60 : &D_800F1BE0;
        } else {
            info = !func_00267FA8(actor) ? &D_800F1B80 : &D_800F1B00;
        }
    }
    if (outInfo != NULL) {
        *outInfo = info;
    }
    saved = actor->motion;
    hit = func_00239F00(actor, pos, info);
    actor->motion = saved;
    return hit;
}
