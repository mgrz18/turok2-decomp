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
    /* 0x1C */ void *player;
    /* 0x20 */ MenuOptions *options;
} MenuItem;

s32 func_00285A68(s8);                          /* extern */

void func_004181A4(MenuItem *arg0) {
    func_00285A68(arg0->options->unk4);
}
