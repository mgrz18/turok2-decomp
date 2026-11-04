#include "common.h"

s32 func_0020185C(s32, s32, s32, s32);

/* The world: the table this looks entries up in. */
typedef struct World {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 base;
    /* 0x14 */ u8 pad14[0x3C - 0x14];
    /* 0x3C */ s32 table;
} World;

void func_00224F48(World *arg0, s32 arg1, s32 *arg2, s32 arg3) {
    *arg2 = func_0020185C(arg0->table, arg0->base, arg1, arg3);
}
