/* NON-MATCHING: 34 of 161 words differ (W batch). */
/* NON-MATCHING: 34 of 161 words differ: callee-saved FP and int registers assigned in a different order (the constants -15, 15, 10.24, 30.72, 512 are hoisted as in the ROM, but into other regs). */
#include "common.h"

/* Drops a pickup (picked from a weighted table) near each of the four spawn
 * points, skipping the points within 460.8 of the boss target; each pickup
 * spawns as func_0040C2C4 does it (inlined). */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u8 padC[0x14 - 0xC];
} SpawnPoint;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f pos;
} PoolActor;

typedef struct {
    /* 0x00 */ void *obj;
} State;

extern State D_8012FB70;
extern SpawnPoint D_00431D20[4];
extern u8 D_800F7078[];
extern u8 D_80107590[];

extern f32 func_0040BD60(void *obj, PoolActor **found);
extern s32 func_0026D518(void *table);
extern f32 func_002119FC(f32 lo, f32 hi);
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);
extern f32 func_00298470(f32);
extern s32 func_00220A1C(void *level, Vec3f *pos);
extern PoolActor *func_00227240(void *pool, s32 *refCount, s32 id, Vec3f vel, Vec3f pos, s32 region, f32 lifetime);
extern void func_00246690(PoolActor *actor, s32 sound, Vec3f pos, s32 arg);

/* func_0040C2C4 */
static __inline__ void spawnPickup(s32 id, s32 lifetime, Vec3f pos, f32 speed) {
    Vec3f vel;
    s32 region;
    PoolActor *pickup;

    region = func_00220A1C(D_800F7078, &pos);
    vel.x = 0.0f;
    vel.y = speed;
    vel.z = 0.0f;
    pickup = func_00227240(D_80107590, NULL, id, vel, pos, region, lifetime);
    if (pickup != NULL) {
        func_00246690(pickup, 0x11D, pos, 0);
    }
}

void func_00409500(s32 lifetime, void *table) {
    PoolActor *found;
    Vec3f pos;
    s32 id;
    s32 i;
    s32 dx;
    s32 dz;
    f32 y;

    func_0040BD60(D_8012FB70.obj, &found);
    for (i = 0; i < 4; i++) {
        y = 30.72f;
        id = (s16) func_0026D518(table);
        dx = func_002119FC(-15.0f, 15.0f);
        dz = func_002119FC(-15.0f, 15.0f);
        dx = dx * 10.24f;
        dz = dz * 10.24f;
        if (found != NULL) {
            func_0020EF2C(&pos, &found->pos, &D_00431D20[i].pos);
            if (!(func_00298470(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z) > 460.8f)) {
                goto next;
            }
        }
        pos = D_00431D20[i].pos;
        pos.y = y;
        pos.x += dx;
        pos.z += dz;
        spawnPickup(id, lifetime, pos, 512.0f);
    next:;
    }
}
