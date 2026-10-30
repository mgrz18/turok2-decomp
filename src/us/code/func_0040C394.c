#include "common.h"

/* Spawns a pickup picked from a weighted table at a random point around the origin (func_0040C110 and
 * func_0040C2C4 inlined). */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 data[0x68 - 0x4];
} Slot;

typedef struct {
    /* 0x000 */ u8 pad0[0x234];
    /* 0x234 */ void *callback234;
    /* 0x238 */ u8 pad238[0x248 - 0x238];
    /* 0x248 */ void *callback248;
    /* 0x24C */ u8 pad24C[0x250 - 0x24C];
    /* 0x250 */ s32 unk250;
    /* 0x254 */ s32 unk254;
} LevelActor;

typedef struct {
    /* 0x000 */ void *obj;
    /* 0x004 */ s32 count;
    /* 0x008 */ LevelActor *actors[4];
    /* 0x018 */ Slot slots[15];
    /* 0x630 */ u8 pad630[0x65C - 0x630];
    /* 0x65C */ s32 phase;
    /* 0x660 */ s32 activeMask;
    /* 0x664 */ s32 unk664;
    /* 0x668 */ u8 pad668[0x674 - 0x668];
    /* 0x674 */ s32 unk674;
    /* 0x678 */ s32 unk678;
    /* 0x67C */ s32 counter;
    /* 0x680 */ u8 pad680[0x688 - 0x680];
    /* 0x688 */ s32 unk688;
    /* 0x68C */ s32 unk68C;
} State;

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f pos;
} PoolActor;

typedef struct {
    s16 id;
    s16 weight;
} Pick;

extern State D_8012FB70;
extern u8 D_800F7078[];
extern u8 D_80107590[];
extern s16 func_0026D518(Pick *table);
extern f32 func_002119FC(f32 min, f32 max);
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern s32 func_00220A1C(void *level, Vec3f *pos);
extern PoolActor *func_00227240(void *pool, s32 *refCount, s32 id, Vec3f vel, Vec3f pos, s32 region, f32 lifetime);
extern void func_00246690(PoolActor *actor, s32 sound, Vec3f pos, s32 arg);

/* func_0040C110 */
static __inline__ void randomGroundPoint(Vec3f *out) {
    State *state = &D_8012FB70;
    f32 dist;
    f32 angle;
    f32 z;

    dist = func_002119FC((f32)state->unk688, (f32)state->unk68C);
    angle = func_002119FC(-3.1415927f, 3.1415927f);
    out->x = dist * func_002982F0(angle);
    z = dist * func_002974C0(angle);
    out->y = 0.0f;
    out->z = z;
}

/* func_0040C2C4 */
static __inline__ void spawnPickup(s32 id, s32 lifetime, Vec3f pos) {
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

void func_0040C394(s32 lifetime, Pick *table) {
    Vec3f pos;
    s32 id;

    id = func_0026D518(table);
    randomGroundPoint(&pos);
    spawnPickup(id, lifetime, pos);
}
