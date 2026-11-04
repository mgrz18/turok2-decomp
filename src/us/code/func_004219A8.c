#include "common.h"

/* A menu item (MenuItem in func_00450F4C) and the options record it points to. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s8 unk4;
} MenuOptions;

typedef struct MenuItem {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[0x14 - 6];
    /* 0x14 */ char *text;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ s32 player;
    /* 0x20 */ MenuOptions *options;
} MenuItem;

extern s32 D_800C2028;
extern s32 D_80130990;

s32 func_004219A8(MenuItem *arg0, s32 *arg1) {
    s32 var_a0;
    s32 var_v0;

    if (D_80130990 != 0) {
        var_v0 = D_800C2028;
        var_a0 = 0;
    } else {
        var_a0 = arg0->player;
        var_v0 = arg0->options->unk4;
    }
    *arg1 = var_v0;
    return var_a0;
}
