#include "common.h"

typedef struct Vec3f { f32 x, y, z; } Vec3f;
typedef struct Quat { f32 x, y, z, w; } Quat;

typedef struct SpawnObj {
    /* 0x000 */ u8 type;            /* 0 = level actor, 1 = game object, 2 = ..., 3 = particle */
    /* 0x001 */ u8 pad1[0x18 - 1];
    /* 0x018 */ Vec3f velocity;
    /* 0x024 */ u8 pad24[0x1C0 - 0x24];
    /* 0x1C0 */ s32 target;
} SpawnObj;

extern u8 D_800F8928[];

void func_002688C8(SpawnObj *obj, Vec3f pos, Vec3f *outPos, s32 *outRegion, s32 x, s32 y);
Quat func_0021AE6C(SpawnObj *obj);
Quat func_002156D4(SpawnObj *obj);
s32 func_0021F1F8(SpawnObj *obj);
s32 func_0022F350(void *table, void *actor, void *parent, void *slot, s32 a, s32 kind, Vec3f origin,
                  Quat rot, Vec3f pos, s32 region, s32 b, s32 c);

/* Spawns a particle of `kind` from `obj` at `at`, moving with it and
 * oriented like it. */
void func_00246690(SpawnObj *obj, s32 kind, Vec3f at) {
    Vec3f pos;
    Vec3f vel;
    Quat rot;
    s32 region;
    s32 mode;
    s32 target;

    mode = 1;
    func_002688C8(obj, at, &pos, &region, 0, 1);
    target = 0;
    switch (obj->type) {
    case 0:
    case 2:
        vel.x = vel.y = vel.z = 0.0f;
        rot.x = rot.y = rot.z = 0.0f;
        rot.w = 1.0f;
        break;
    case 1:
        vel = obj->velocity;
        rot = func_0021AE6C(obj);
        target = obj->target;
        if (func_0021F1F8(obj) == 5) {
            mode = 3;
        }
        break;
    case 3:
        vel = obj->velocity;
        rot = func_002156D4(obj);
        break;
    }
    func_0022F350(D_800F8928, obj, obj, NULL, target, kind, vel, rot, pos, region, -1, mode);
}
