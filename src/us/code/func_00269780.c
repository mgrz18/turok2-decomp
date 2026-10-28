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
} Actor;

typedef struct CollisionInfo {
    /* 0x00 */ s32 flags;
} CollisionInfo;

extern CollisionInfo D_800F1AE0;

f32 func_002671B4(Actor *actor);    /* height */
s32 func_00239F00(Actor *actor, Vec3 pos, CollisionInfo *info);

/* Whether actor can see target: a clear line from 80% of one's height to 80% of the other's. */
s32 func_00269780(Actor *actor, Actor *target) {
    ActorMotion saved;
    Vec3 to;
    s32 hit;

    saved = actor->motion;
    actor->motion.pos.y += func_002671B4(actor) * 0.8f;
    to = target->motion.pos;
    to.y += func_002671B4(target) * 0.8f;
    hit = func_00239F00(actor, to, &D_800F1AE0);
    actor->motion = saved;
    return hit == 0;
}
