#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u8 *player;
} MenuCaller;

extern s32 D_801198A0;
extern char D_800F6CB0[];
extern void func_00430330(char *);

s32 func_0041A3D4(s32 menu, MenuCaller *c) {
    if (D_801198A0 == 1) {
        func_00430330(D_800F6CB0);
    } else {
        *(s32 *) (c->player + 0x510) = 0;
    }
    return 1;
}
