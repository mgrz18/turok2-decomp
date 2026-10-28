#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

extern f32 func_002119FC(f32 min, f32 max); /* random float in [min, max] */
extern f32 func_002465F0(void *obj, Vec3f v);
extern f32 func_00298470(f32 x); /* sqrtf */
extern f32 func_002982F0(f32 x); /* sinf */
extern f32 func_002974C0(f32 x); /* cosf */

extern f32 D_800F1F98; /* gravity */

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
} Target;

typedef struct {
    /* 0x000 */ u8 pad0[0x50];
    /* 0x050 */ f32 angle;
    /* 0x054 */ u8 pad54[0x120];
    /* 0x174 */ s8 mode;
    /* 0x175 */ u8 pad175[0x4B];
    /* 0x1C0 */ Target *target;
} OwnerActor;

typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ OwnerActor *owner;
} OwnedState;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3f vel;
} Object;

/* Launch velocity: thrown away from the owner, aimed at its target. */
void func_00264F58(Object *obj, OwnedState *state) {
    OwnerActor *owner = state->owner;
    Target *target;
    f32 speed;
    f32 lift;
    f32 r;
    f32 x;

    if (owner->mode != 0x15) {
        speed = 51.199997f;
        lift = 204.79999f;
    } else if ((target = owner->target) != NULL) {
        r = func_002119FC(1.5f, 2.5f);
        speed = r * func_00298470(func_002465F0(obj, target->pos));
        lift = -D_800F1F98 / (r * 3.8f);
    } else {
        speed = 512.0f;
        lift = 358.4f;
    }
    lift += func_002119FC(0.0f, 2.56f);
    x = speed * func_002982F0(owner->angle + 3.1415927f);
    obj->vel.y = lift;
    obj->vel.x = x;
    obj->vel.z = speed * func_002974C0(owner->angle + 3.1415927f);
}
