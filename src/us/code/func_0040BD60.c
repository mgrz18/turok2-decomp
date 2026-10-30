#include "common.h"

/* Returns the distance to the AI's second target when it is an actor with flag 0x300000 (and stores that
 * actor in *found), else -1 (and NULL). */

typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ s32 flags;
} Actor;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Actor *actor;
    /* 0x08 */ u8 pad8[0x40 - 0x8];
    /* 0x40 */ f32 dist;
} Target;

typedef struct {
    /* 0x00 */ Target a;
    /* 0x44 */ Target b;
} Targets;

typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Obj;

extern void func_00246108(Obj *obj, void *ai, Targets *t, s32 f1, s32 f2);

f32 func_0040BD60(Obj *obj, Actor **found) {
    Targets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    func_00246108(obj, obj->ai, &t, 1, 0);
    if (t.b.actor != NULL && (t.b.actor->flags & 0x300000)) {
        dist = t.b.dist;
        *found = t.b.actor;
    }
    return dist;
}
