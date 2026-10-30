#include "common.h"

/* Spawns pool pickup id at pos, thrown upward, lasting the given time, and plays its spawn sound. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f pos;
} PoolActor;

extern u8 D_800F7078[];
extern u8 D_80107590[];
extern s32 func_00220A1C(void *level, Vec3f *pos);
extern PoolActor *func_00227240(void *pool, s32 *refCount, s32 id, Vec3f vel, Vec3f pos, s32 region, f32 lifetime);
extern void func_00246690(PoolActor *actor, s32 sound, Vec3f pos, s32 arg);

void func_0040C2C4(s32 id, s32 lifetime, Vec3f pos) {
    Vec3f vel;
    s32 region;
    PoolActor *pickup;

    region = func_00220A1C(D_800F7078, &pos);
    vel.x = 0.0f;
    vel.y = 512.0f;
    vel.z = 0.0f;
    pickup = func_00227240(D_80107590, NULL, id, vel, pos, region, lifetime);
    if (pickup != NULL) {
        func_00246690(pickup, 0x11D, pos, 0);
    }
}
