#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

/* The leading 0x34 bytes of a game object, as the collision test reads them. */
typedef struct ObjectHead {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ void *region;
    /* 0x14 */ u8 pad14[0x4];
    /* 0x18 */ Vec3 velocity;
    /* 0x24 */ u8 pad24[0x10];
} ObjectHead; /* size 0x34 */

typedef struct CollisionInfo {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 pad6[0x1C - 0x6];
} CollisionInfo; /* size 0x1C */

/* The result of the last collision test. */
typedef struct CollisionResult {
    /* 0x00 */ void *hit;
    /* 0x04 */ u8 pad4[0x9C - 0x4];
    /* 0x9C */ void *hit2;
} CollisionResult;

extern CollisionResult D_800F1CE0;

s32 func_00239F00(ObjectHead *obj, Vec3 to, CollisionInfo *info);

/* Whether obj can move to `to` without hitting anything; obj is left where it was. */
s32 func_002698AC(ObjectHead *obj, Vec3 to, CollisionInfo *info) {
    ObjectHead saved;
    CollisionInfo probe;
    s32 hit;

    saved = *obj;
    probe = *info;
    probe.unk5 = 1;
    probe.unk4 = 0;
    func_00239F00(obj, to, &probe);
    *obj = saved;
    hit = 0;
    if (D_800F1CE0.hit != NULL || D_800F1CE0.hit2 != NULL) {
        hit = 1;
    }
    return hit ^ 1;
}
