#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* Spawns a pickup picked from a weighted table at a random point around the origin (func_0040C110 and
 * func_0040C2C4 inlined). */

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ CVector3 pos;
} PoolActor;

typedef struct {
    s16 id;
    s16 weight;
} Pick;

extern u8 D_800F7078[];
extern u8 D_80107590[];
extern s16 func_0026D518(Pick *table);
extern f32 func_002119FC(f32 min, f32 max);
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern s32 func_00220A1C(void *level, CVector3 *pos);
extern PoolActor *func_00227240(void *pool, s32 *refCount, s32 id, CVector3 vel, CVector3 pos, s32 region, f32 lifetime);
extern void func_00246690(PoolActor *actor, s32 sound, CVector3 pos, s32 arg);

/* func_0040C110 */
static __inline__ void randomGroundPoint(CVector3 *out) {
    MotherBossState *state = &D_8012FB70;
    f32 dist;
    f32 angle;
    f32 z;

    dist = func_002119FC((f32)state->minRadius, (f32)state->maxRadius);
    angle = func_002119FC(-3.1415927f, 3.1415927f);
    out->x = dist * func_002982F0(angle);
    z = dist * func_002974C0(angle);
    out->y = 0.0f;
    out->z = z;
}

/* func_0040C2C4 */
static __inline__ void spawnPickup(s32 id, s32 lifetime, CVector3 pos) {
    CVector3 vel;
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
    CVector3 pos;
    s32 id;

    id = func_0026D518(table);
    randomGroundPoint(&pos);
    spawnPickup(id, lifetime, pos);
}
