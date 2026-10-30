#include "common.h"

s32 func_00284174(void);
s32 func_00284240(void);
void func_00416644(char *dst, char *fmt, s32 arg2, s32 arg3, s32 arg4);

extern char D_0043B714[];
extern char D_80119E2C[];
extern s32 D_80130980;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 player;
    /* 0x20 */ s32 arg;
} MenuEvent;

static __inline__ s32 func_00422EE0_inline(void) {
    if (func_00284174() != 0 && func_00284240() == 0) {
        return 1;
    }
    return 0;
}

s32 func_00423178(s32 unused, MenuEvent *event) {
    s32 player = event->player;

    if (func_00422EE0_inline()) {
        return 0;
    }
    if (player != 0) {
        D_80130980 = 1;
        func_00416644(D_80119E2C, D_0043B714, event->player, event->arg, 0);
        return 1;
    }
    return 0;
}
