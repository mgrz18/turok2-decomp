#include "common.h"

/* Module update: ticks every slot, then (outside cinematics) counts the two phase timers: the first spawns
 * one actor at a random spot around the origin, the second starts a burst of spawns at the phase points. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 data[0x68 - 0x4];
} Slot;

typedef struct {
    /* 0x000 */ void *obj;
    /* 0x004 */ s32 count;
    /* 0x008 */ void *actors[4];
    /* 0x018 */ Slot slots[15];
    /* 0x630 */ u8 pad630[0x65C - 0x630];
    /* 0x65C */ s32 phase;
    /* 0x660 */ u8 pad660[0x680 - 0x660];
    /* 0x680 */ s32 spawnTimer;
    /* 0x684 */ s32 burstTimer;
    /* 0x688 */ s32 minRadius;
    /* 0x68C */ s32 maxRadius;
} State;

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
extern s32 func_00220A1C(void *world, Vec3f *pos);
extern void *func_00227240(void *pool, s32 a1, s16 type, Vec3f vel, Vec3f pos, s32 region, f32 arg);
extern void func_00246690(void *actor, s32 id, Vec3f pos, s32 a5);
extern void func_00409500(s32 delay, void *points);

static __inline__ void SpawnAt(s32 delay, s16 type, Vec3f pos, f32 speed) {
    Vec3f vel;
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

void func_00442784(State *state) {
    Vec3f v;
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
        func_00235510(state->slots[i].data);
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    state->spawnTimer++;
    state->burstTimer++;
    switch (state->phase) {
        case 1:
            if (state->spawnTimer >= 76) {
                spawn = 1;
                delay = 600;
            }
            if (state->burstTimer >= 526) {
                burst = 1;
                points = D_00431CE4;
                count = 330;
            }
            break;
        case 2:
            if (state->spawnTimer >= 151) {
                spawn = 1;
                delay = 450;
            }
            if (state->burstTimer >= 526) {
                burst = 1;
                points = D_00431CF0;
                count = 330;
            }
            break;
        case 0:
            if (state->spawnTimer >= 151) {
                spawn = 1;
                delay = 450;
            }
            break;
    }
    if (Cinema_PathPlaying()) {
        return;
    }
    if (spawn) {
        state->spawnTimer = 0;
        type = (s16)func_0026D518(D_00431CB4);
        r = func_002119FC(D_801301F8, D_801301FC);
        angle = func_002119FC(-3.1415927f, 3.1415927f);
        v.x = r * func_002982F0(angle);
        v.z = r * func_002974C0(angle);
        v.y = 0.0f;
        SpawnAt(delay, type, v, 512.0f);
    }
    if (points != NULL && burst) {
        state->burstTimer = 0;
        func_00409500(count, points);
    }
}
