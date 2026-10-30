#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 pad1[0x140 - 0xD8];
    /* 0x140 */ u8 ai[0x207 - 0x140];
    /* 0x207 */ s8 active;
    /* 0x208 */ u8 pad2[0x24C - 0x208];
    /* 0x24C */ s32 state;
} Actor00403430;

typedef struct {
    /* 0x000 */ u8 pad0[0x220];
    /* 0x220 */ Actor00403430 *actor;
} Owner00403430;

void func_002666B0(Actor00403430 *actor, void *ai, s32 state);

void func_00403430(Owner00403430 *owner) {
    Actor00403430 *actor = owner->actor;
    s32 state;

    actor->flags &= ~0x10000000;
    state = actor->state;
    if (state == 0x5AB4) {
    } else if (state < 0x5AB5) {
    } else if (state == 0x5ABE) {
        if (actor->active != 0) {
            func_002666B0(owner->actor, owner->actor->ai, 0x5ADC);
        }
    }
}
