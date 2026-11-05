#include "common.h"
#include "structs/vector.h"
#include "structs/boss.h"

/* Boss fight tick: updates the 15 attack slots, then (outside cinematics)
 * counts up the two timers: by phase, drops a random pickup somewhere in the
 * arena (inlined func_0040C2C4) and, later, a pickup at every spawn point
 * (func_00409500). */

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ CVector3 pos;
} PoolActor;

extern s32 D_801301F8;      /* arena radius range */
extern s32 D_801301FC;
extern u8 D_00431D94[];     /* pickup weights */
extern u8 D_00431DC4[];
extern u8 D_00431DD0[];
extern u8 D_800F7078[];
extern u8 D_80107590[];

extern void func_00235510(void *slot);
extern s32 Cinema_PathPlaying(void);
extern s32 func_0026D518(void *table);
extern f32 func_002119FC(f32 lo, f32 hi);
extern f32 func_002982F0(f32);
extern f32 func_002974C0(f32);
extern s32 func_00220A1C(void *level, CVector3 *pos);
extern PoolActor *func_00227240(void *pool, s32 *refCount, s32 id, CVector3 vel, CVector3 pos, s32 region, f32 lifetime);
extern void func_00246690(PoolActor *actor, s32 sound, CVector3 pos, s32 arg);
extern void func_00409500(s32 lifetime, void *table);

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

void func_00409784(MotherBossState *s) {
    CVector3 pos;
    s32 i;
    s32 spawn;
    s32 lifetime;
    s32 drop;
    s32 dropLifetime;
    void *table;
    s32 id;
    f32 dist;
    f32 angle;

    spawn = 0;
    drop = 0;
    lifetime = 0;
    dropLifetime = 0;
    table = NULL;
    for (i = 0; i < 15; i++) {
        func_00235510(&s->slots[i].anim);
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    s->pickupTimer++;
    s->dropTimer++;
    switch (s->phase) {
    case 1:
        if (s->pickupTimer >= 76) {
            spawn = 1;
            lifetime = 600;
        }
        if (s->dropTimer >= 526) {
            drop = 1;
            table = D_00431DC4;
            dropLifetime = 330;
        }
        break;
    case 2:
        if (s->pickupTimer >= 151) {
            spawn = 1;
            lifetime = 450;
        }
        if (s->dropTimer >= 526) {
            drop = 1;
            table = D_00431DD0;
            dropLifetime = 330;
        }
        break;
    case 0:
        if (s->pickupTimer >= 151) {
            spawn = 1;
            lifetime = 450;
        }
        break;
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    if (spawn) {
        s->pickupTimer = 0;
        id = (s16) func_0026D518(D_00431D94);
        dist = func_002119FC(D_801301F8, D_801301FC);
        angle = func_002119FC(-3.1415927f, 3.1415927f);
        pos.x = dist * func_002982F0(angle);
        pos.z = dist * func_002974C0(angle);
        pos.y = 0.0f;
        spawnPickup(id, lifetime, pos);
    }
    if (table != NULL && drop) {
        s->dropTimer = 0;
        func_00409500(dropLifetime, table);
    }
}
