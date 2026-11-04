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

s32 func_00416644(s32, s32, s32, s32, s32);

s32 func_00285A68(s8);                          /* extern */
extern s32 D_0043BD44;
extern s32 D_80119E2C;

s32 func_004195C0(s32 arg0, MenuItem *arg1) {
    func_00285A68(arg1->options->unk4);
    func_00416644((s32) &D_80119E2C, (s32) &D_0043BD44, arg1->player, (s32) arg1->options, 0);
    return 1;
}
