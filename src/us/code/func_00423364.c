#include "common.h"
#include "structs/menu.h"

s32 func_00284174(void);
s32 func_00284240(void);
s32 func_002484F8(u8 *player, s32 arg1, s32 arg2);
void func_00412B14(void);
void func_00412438(s32 sound);

static __inline__ s32 func_00422EE0_inline(void) {
    if (func_00284174() != 0 && func_00284240() == 0) {
        return 1;
    }
    return 0;
}

s32 func_00423364(s32 unused, Menu *event) {
    u8 *player = event->player;
    s32 ok;

    if (func_00422EE0_inline()) {
        return 0;
    }
    ok = func_002484F8(player, 0, *(s32 *)(player + 0x92C));
    event->cursor = 0;
    if (ok) {
        func_00412B14();
        func_00412438(0x193);
    }
    return 0;
}
