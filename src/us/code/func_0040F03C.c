#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f pos;
} Actor;

extern u8 D_8010ABA0[]; /* world */

extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */
extern void func_002688C8(Actor *actor, Vec3f pos, Vec3f *outPos, s32 *outRegion, s32 flagA, s32 flagB);
extern void *func_00228108(void *world, s32 model, s32 type, void *intelligence, s32 region, Vec3f pos, f32 rotY,
                           Vec3f vel, Vec3f scale, s32 *owner);

/* Spawns an actor at `dist` from `origin` in direction `angle`, if the budget allows it. */
void func_0040F03C(Actor **self, s32 *budget, s32 *spawned, s32 max, void *intelligence, Actor *origin, f32 dist,
                   f32 angle) {
    Vec3f pos;
    Vec3f vel;
    Vec3f scale;
    s32 region;

    if (budget != NULL && *budget == 0) {
        return;
    }
    if (spawned != NULL && max != -1 && *spawned >= max) {
        return;
    }
    vel.x = 0.0f;
    vel.y = 0.0f;
    vel.z = 0.0f;
    scale.x = 0.5f;
    scale.y = 0.5f;
    scale.z = 0.5f;
    pos.x = origin->pos.x + dist * func_002982F0(angle);
    pos.y = 0.0f;
    pos.z = origin->pos.z + dist * func_002974C0(angle);
    func_002688C8(*self, pos, &pos, &region, 0, 0);
    if (func_00228108(D_8010ABA0, -1, 0x1FF, intelligence, region, pos, angle, vel, scale, spawned) != NULL &&
        budget != NULL) {
        (*budget)--;
    }
}
