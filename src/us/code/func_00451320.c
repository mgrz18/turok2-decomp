#include "common.h"
#include "structs/menu.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ s8 unk30;
} Settings;

typedef struct {
    /* 0x000 */ u8 pad0[0x514];
    /* 0x514 */ s32 unk514;
    /* 0x518 */ Settings *settings;
    /* 0x51C */ s32 unk51C;
    /* 0x520 */ u8 pad520[0xA50 - 0x520];
    /* 0xA50 */ s32 controller;
    /* 0xA54 */ u8 padA54[0xBF4 - 0xA54];
    /* 0xBF4 */ s32 unkBF4;
} Player;

s32 func_00416644(s32, s32, s32, s32, s32);
extern s32 D_00438728;

s32 func_00451320(s32 arg0, Menu *arg1) {
    Player *temp_a2;

    temp_a2 = arg1->player;
    temp_a2->unkBF4 = 0;
    temp_a2->settings->unk30 = 0;
    func_00416644(temp_a2->unk51C + 0x53C, (s32) &D_00438728, (s32) temp_a2, temp_a2->controller, temp_a2->unk514);
    return 1;
}
