#include "common.h"

typedef struct MenuContext {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ void *player;
} MenuContext;

void func_0042E06C(void *player);

s32 func_00453310(void *item, MenuContext *menu) {
    func_0042E06C(menu->player);
    return 1;
}
