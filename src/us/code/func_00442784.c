#include "common.h"
#include "structs/vector.h"
#include "structs/boss.h"

/* Module update: ticks every slot, then (outside cinematics) counts the two phase timers: the first spawns
 * one actor at a random spot around the origin, the second starts a burst of spawns at the phase points. */

extern s32 D_801301F8; /* D_8012FB70.minRadius */
extern s32 D_801301FC; /* D_8012FB70.maxRadius */
extern u8 D_00431CB4[];
extern u8 D_00431CE4[];
extern u8 D_00431CF0[];
extern u8 D_800F7078[];
extern u8 D_80107590[];
extern void func_00235510(void *slot);
extern s32 Cinema_PathPlaying(void);
extern s32 func_0026D518(void *a);
extern f32 func_002119FC(f32 min, f32 max);
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern s32 func_00220A1C(void *world, CVector3 *pos);
extern void *func_00227240(void *pool, s32 a1, s16 type, CVector3 vel, CVector3 pos, s32 region, f32 arg);
extern void func_00246690(void *actor, s32 id, CVector3 pos, s32 a5);
extern void func_00409500(s32 delay, void *points);

static __inline__ void SpawnAt(s32 delay, s16 type, CVector3 pos, f32 speed) {
    CVector3 vel;
    s32 region;
    void *actor;

    region = func_00220A1C(D_800F7078, &pos);
    vel.x = 0.0f;
    vel.y = speed;
    vel.z = 0.0f;
    actor = func_00227240(D_80107590, 0, type, vel, pos, region, delay);
    if (actor != NULL) {
        func_00246690(actor, 0x11D, pos, 0);
    }
}

void func_00442784(MotherBossState *state) {
    CVector3 v;
    s32 i;
    s32 spawn = 0;
    s32 burst = 0;
    s32 delay = 0;
    s32 count = 0;
    void *points = NULL;
    s32 type;
    f32 r;
    f32 angle;

    for (i = 0; i < 15; i++) {
        func_00235510(&state->slots[i].anim);
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    state->pickupTimer++;
    state->dropTimer++;
    switch (state->phase) {
        case 1:
            if (state->pickupTimer >= 76) {
                spawn = 1;
                delay = 600;
            }
            if (state->dropTimer >= 526) {
                burst = 1;
                points = D_00431CE4;
                count = 330;
            }
            break;
        case 2:
            if (state->pickupTimer >= 151) {
                spawn = 1;
                delay = 450;
            }
            if (state->dropTimer >= 526) {
                burst = 1;
                points = D_00431CF0;
                count = 330;
            }
            break;
        case 0:
            if (state->pickupTimer >= 151) {
                spawn = 1;
                delay = 450;
            }
            break;
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    if (spawn) {
        state->pickupTimer = 0;
        type = (s16)func_0026D518(D_00431CB4);
        r = func_002119FC(D_801301F8, D_801301FC);
        angle = func_002119FC(-3.1415927f, 3.1415927f);
        v.x = r * func_002982F0(angle);
        v.z = r * func_002974C0(angle);
        v.y = 0.0f;
        SpawnAt(delay, type, v, 512.0f);
    }
    if (points != NULL && burst) {
        state->dropTimer = 0;
        func_00409500(count, points);
    }
}
