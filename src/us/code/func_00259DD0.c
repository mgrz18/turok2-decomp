#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ void *owner;
} Actor;

s32 func_0024E700(void *player, s32 weapon);
void func_00243414(Actor *actor, void *state, s32 mode);

void func_00259DD0(Actor *actor, void *state) {
    if (func_0024E700(actor->owner, 0x1F) == 1) {
        func_00243414(actor, state, 7);
    } else {
        func_00243414(actor, state, 4);
    }
}
