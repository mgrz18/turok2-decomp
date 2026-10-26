/* A hinged/rotating actor: swing open and shut (mode 1) or turn toward a target (mode 2). */

#include "common.h"

typedef struct { f32 x, y, z; } Vec3;

typedef struct SwingParams {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ f32 height;
    /* 0x5C */ f32 radius;
    /* 0x60 */ f32 heading;
    /* 0x64 */ f32 speed;
} SwingParams;

typedef struct SwingDef {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ SwingParams params;
} SwingDef;

typedef struct SwingOwner {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ SwingDef *def;
} SwingOwner;

typedef struct SwingActor {
    /* 0x000 */ u8 pad0[0x37];
    /* 0x037 */ s8 state;
    /* 0x038 */ u8 pad38[0x54 - 0x38];
    /* 0x054 */ u8 rotation[0x64 - 0x54];
    /* 0x064 */ f32 open;
    /* 0x068 */ u8 pad68[0x9C - 0x68];
    /* 0x09C */ Vec3 pos;
    /* 0x0A8 */ u8 padA8[0x110 - 0xA8];
    /* 0x110 */ f32 velocity;
} SwingActor;

typedef struct SwingTarget {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3 pos;
} SwingTarget;

extern f32 func_00298470(f32);                          /* sqrtf */
extern f32 func_002982F0(f32);                          /* sinf */
extern f32 func_002974C0(f32);                          /* cosf */
extern f32 func_00211774(f32 cur, f32 target, f32 rate);
extern void func_0020EF2C(Vec3 *out, Vec3 *a, Vec3 *b);
extern void func_002105A4(f32 mtx[4][4], f32 angle);
extern void func_0020F85C(f32 mtx[4][4], Vec3 *in, Vec3 *out);
extern void func_0020EF8C(void *rotation, f32 angle, Vec3 *pivot, Vec3 *point);

void func_00262150(SwingOwner *owner, SwingActor *actor, s32 mode, SwingTarget *target) {
    SwingParams *p;
    Vec3 hinge;
    Vec3 delta;
    Vec3 local;
    f32 mtx[4][4];
    f32 open;
    f32 maxSpeed;
    f32 speed;
    f32 desired;
    f32 radius;
    f32 velocity;

    p = &owner->def->params;
    open = actor->open;
    radius = p->radius;
    if (radius == 0.0f && p->height == 0.0f) {
        maxSpeed = 0.0f;
    } else {
        maxSpeed = p->speed / func_00298470(radius * radius + p->height * p->height);
    }

    switch (mode) {
        case 1:
            speed = maxSpeed;
            if (actor->state == mode) {
                speed = -speed;
            }
            velocity = func_00211774(actor->velocity, speed, 0.25f);
            open += velocity;
            actor->velocity = velocity;
            if (actor->state == 0) {
                if (open > 1.0f) {
                    actor->velocity = 0.0f;
                    actor->state = mode;
                }
            } else {
                if (open < 0.0f) {
                    actor->velocity = 0.0f;
                    actor->state = 0;
                }
            }
            break;
        case 0:
            break;
        case 2:
            if (target != NULL) {
                if (p->heading == 0.0f) {
                    desired = 0.0f;
                } else {
                    func_0020EF2C(&delta, &target->pos, &actor->pos);
                    func_002105A4(mtx, -p->heading);
                    func_0020F85C(mtx, &delta, &local);
                    desired = local.z / p->radius;
                }
                speed = desired - open;
                if (speed > maxSpeed) {
                    speed = maxSpeed;
                } else if (speed < -maxSpeed) {
                    speed = -maxSpeed;
                }
                velocity = func_00211774(actor->velocity, speed, 0.25f);
                open += velocity;
                actor->velocity = velocity;
            }
            break;
    }

    if (open > 1.0f) {
        open = 1.0f;
    } else if (open < 0.0f) {
        open = 0.0f;
    }
    actor->open = open;

    hinge = actor->pos;
    hinge.x += p->radius * func_002982F0(p->heading);
    hinge.y += p->height;
    hinge.z += p->radius * func_002974C0(p->heading);
    func_0020EF8C(actor->rotation, open, &actor->pos, &hinge);
}
