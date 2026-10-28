#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

extern f32 func_0024665C(void *actor, Vec3f point); /* squared XZ distance */
extern f32 func_002672C8(void *actor);              /* collision radius */
extern void func_0025E488(void *actor, void *ai, s32 arg2);
extern void func_00243414(void *obj, void *state, s32 mode);
extern f32 func_00298470(f32 x); /* sqrtf */

extern s8 D_800F1F94[];

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
} OwnerActor;

typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ OwnerActor *owner;
    /* 0x110 */ f32 tiltAngle;
} OwnedState;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3f vel;
} Object;

void func_0026508C(Object *obj, OwnedState *state) {
    OwnerActor *owner = state->owner;
    f32 distSq;

    if (owner != NULL) {
        D_800F1F94[0] = 0;
        distSq = func_0024665C(obj, owner->pos);
        if ((func_002672C8(obj) + func_002672C8(owner)) * (func_002672C8(obj) + func_002672C8(owner)) < distSq) {
            state->owner = NULL;
            func_0025E488(owner, (u8 *) owner + 0x140, 0);
        }
    } else {
        D_800F1F94[0] = 2;
    }
    if (obj->vel.x == 0.0f && obj->vel.y == 0.0f && obj->vel.z == 0.0f) {
        func_00243414(obj, state, 0);
    }
    state->tiltAngle += func_00298470(obj->vel.x * obj->vel.x + obj->vel.y * obj->vel.y + obj->vel.z * obj->vel.z)
                        * 0.00021305290f;
}
