#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct ObjDef {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x14];
    /* 0x18 */ u16 shape;
} ObjDef;

typedef struct Actor {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
    /* 0x10 */ void *node;
    /* 0x14 */ ObjDef *def;
    /* 0x18 */ u8 pad18[0x30 - 0x18];
    /* 0x30 */ struct Actor *standOn;
} Actor;

f32 func_0026BE60(void *node, f32 x, f32 z);
f32 func_002425A4(void *actor, f32 x, f32 z);
void func_00268798(Actor *actor, Vec3 *velocity);

/* func_00268688, inlined: floor height under (x, z) for an actor. */
static __inline__ f32 actorFloor(Actor *actor, f32 x, f32 z) {
    ObjDef *def;

    if (actor->kind == 1) {
        def = actor->def;
        if (def->type == 2) {
            switch (def->shape) {
                case 0:
                    if (actor->node != NULL) {
                        return func_0026BE60(actor->node, x, z);
                    }
                    return actor->y;
                case 1:
                case 2:
                    return func_002425A4(actor, x, z);
            }
        }
    }
    return actor->y;
}

/* The platform an actor stands on, if it is still touching it. */
Actor *func_00267800(Actor *actor) {
    Actor *platform;
    Vec3 velocity;
    f32 floor;
    f32 gap;

    platform = NULL;
    if (actor->kind == 1) {
        platform = actor->standOn;
    }
    if (platform == NULL) {
        return NULL;
    }
    if (platform->def->type != 2) {
        return platform;
    }
    floor = actorFloor(platform, actor->x, actor->z);
    func_00268798(platform, &velocity);
    gap = actor->y - floor + velocity.y;
    if ((gap < 0.0f) ? (-gap <= 5.12f) : (gap <= 5.12f)) {
        return platform;
    }
    return NULL;
}
