#include "common.h"

typedef void (*AICallback)(void *actor, void *ai);

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ void *table;
    /* 0x030 */ u8 pad30[0xF0 - 0x30];
    /* 0x0F0 */ AICallback update;
    /* 0x0F4 */ u8 padF4[4];
    /* 0x0F8 */ AICallback draw;
    /* 0x0FC */ u8 padFC[0x104 - 0xFC];
    /* 0x104 */ AICallback hit;
    /* 0x108 */ AICallback unk108;
} AI;

extern u8 D_800B1DE0[];
void func_002627B8(void *actor, void *ai);
void func_00262BE0(void *actor, void *ai);
void func_00262A1C(void *actor, void *ai);
void func_00262B58(void *actor, void *ai);

/* AI init: installs this actor kind's table and callbacks. */
void func_002626A8(void *actor, AI *ai) {
    /* FAKEMATCH: the ROM builds func_00262B58's address in $a0 (the dead
     * actor argument) while $v0 still holds func_00262A1C's; no natural
     * source shape tried (temporaries, inline setters, reusing the
     * parameter, store order, permuter) gives a second register other
     * than $v1, so the value is pinned to $a0. */
    register AICallback unk108 asm("$4");

    ai->table = D_800B1DE0;
    ai->update = func_002627B8;
    ai->draw = func_00262BE0;
    ai->hit = func_00262A1C;
    unk108 = func_00262B58;
    ai->unk108 = unk108;
}
