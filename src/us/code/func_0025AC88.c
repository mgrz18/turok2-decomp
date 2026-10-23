#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x76];
    /* 0x76 */ u16 anim;
} ActorDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActorDef *def;
} Actor;

/* Three override slots, 6 bytes apart. */
extern u16 D_800F5C66[];
extern u8 D_800B0630[];
extern u8 D_800AF6F0[];
s32 func_00245A98(Actor *actor, void *state, void *table, s32 anim);

s32 func_0025AC88(Actor *actor, void *state) {
    u16 *slots = D_800F5C66;
    s32 r;

    if (slots[0] != 0) {
        r = func_00245A98(actor, state, D_800B0630, slots[0]);
    } else if (slots[3] != 0) {
        r = func_00245A98(actor, state, D_800B0630, slots[3]) + 100;
    } else if (slots[6] == 0) {
        r = func_00245A98(actor, state, D_800AF6F0, actor->def->anim);
    } else {
        r = func_00245A98(actor, state, D_800B0630, slots[6]) + 200;
    }
    return r;
}
