#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ void *player;
} MenuCaller;

extern void func_0042E14C(void *player);

s32 func_0041C9DC(s32 menu, MenuCaller *c) {
    func_0042E14C(c->player);
    return 1;
}
