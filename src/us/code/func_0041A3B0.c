/* WRONG CUT: real function 0x0041A3B0, size 0x24: splat started it 4 bytes early at a
 * stray word (0x41A3AC, a `bc1f` encoding never executed; func_0041A3A8 is the nop
 * padding before it). */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ void *player;
} MenuCaller;

extern void func_0042E14C(void *player);

s32 func_0041A3B0(s32 menu, MenuCaller *c) {
    func_0042E14C(c->player);
    return 1;
}
