#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ void *player;
} MenuCaller;

s32 func_0042E06C(void *player);                         /* extern */

s32 func_0045593C(s32 menu, MenuCaller *c) {
    func_0042E06C(c->player);
    return 1;
}
