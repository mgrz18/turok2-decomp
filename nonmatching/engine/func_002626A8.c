/* NON-MATCHING: 4 of 16 words differ: the ROM loads func_00262B58's address into $a0 before storing func_00262A1C's (two live temporaries, $v0 and $a0); this C reuses $v0. */
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
    ai->table = D_800B1DE0;
    ai->update = func_002627B8;
    ai->draw = func_00262BE0;
    ai->hit = func_00262A1C;
    ai->unk108 = func_00262B58;
}
