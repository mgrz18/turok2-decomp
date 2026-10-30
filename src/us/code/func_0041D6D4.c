/* WRONG CUT: real function 0x0041D6D4, size 0x98 (through 0x0041D76C): splat cut it in two
 * (func_0041D6D4, func_0041D740) between a lui and its store. */
#include "common.h"

typedef struct {
    /* 0x00 */ s32 cursor;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 length;
    /* 0x0C */ u8 name[12];
} NameEntry; /* 0x18 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ void *player;
} MenuCaller;

extern NameEntry D_800C1FB0[];
extern u8 D_80119870[];
extern s32 func_0025340C(void *players, void *player);

/* Name entry: delete the last character. */
s32 func_0041D6D4(s32 menu, MenuCaller *c) {
    s32 i = func_0025340C(D_80119870, c->player);

    D_800C1FB0[i].unk4 = 0;
    if (D_800C1FB0[i].length == 0) {
        return 0;
    }
    D_800C1FB0[i].length--;
    D_800C1FB0[i].cursor--;
    D_800C1FB0[i].name[D_800C1FB0[i].cursor] = 0;
    return 0;
}
