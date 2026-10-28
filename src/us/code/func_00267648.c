#include "common.h"

#define MAX(a, b) ((a) < (b) ? (b) : (a))

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

static __inline__ f32 nodeFloor(Actor *actor, f32 x, f32 z) {
    void *node = actor->node;
    f32 result;

    if (node == NULL) {
        result = actor->y;
    } else {
        result = func_0026BE60(node, x, z);
    }
    return result;
}

/* func_00268688, inlined: floor height under (x, z) for an actor. */
static __inline__ f32 actorFloor(Actor *actor, f32 x, f32 z) {
    ObjDef *def;

    if (actor->kind == 1) {
        def = actor->def;
        if (def->type == 2) {
            switch (def->shape) {
                case 0:
                    return nodeFloor(actor, x, z);
                case 1:
                case 2:
                    return func_002425A4(actor, x, z);
            }
        }
    }
    return actor->y;
}

static __inline__ Actor *standOn(Actor *actor) {
    Actor *result = NULL;

    if (actor->kind == 1) {
        result = actor->standOn;
    }
    return result;
}

/* Ground height under an actor: its own region floor, or what it stands on if higher. */
f32 func_00267648(Actor *actor) {
    f32 ground;

    if (actor->node == NULL) {
        return actor->y;
    }
    ground = func_0026BE60(actor->node, actor->x, actor->z);
    if (standOn(actor) == NULL) {
        return ground;
    }
    return MAX(ground, actorFloor(standOn(actor), actor->x, actor->z));
}
