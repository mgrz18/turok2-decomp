#include "common.h"
#include "structs/player.h"

/* What a menu item shows: flags at 0x58 (0x01000000: greyed out). */
typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ s32 flags;
} MenuWidget;

/* A menu item (MenuItem in func_00450F4C): its widget at 0xC and its player at 0x1C. */
typedef struct MenuItem {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ MenuWidget *widget;
    /* 0x10 */ u8 pad10[0x1C - 0x10];
    /* 0x1C */ CPlayer *player;
} MenuItem;

s32 func_00253640(s32, s32, s32);

s32 func_004232FC(s32 arg0, MenuItem *arg1) {
    CPlayer *temp_a0;
    MenuWidget *temp_a0_2;
    MenuWidget *temp_v0;

    temp_a0 = arg1->player;
    if (func_00253640((s32) temp_a0, 0, temp_a0->m_Data.m_CheckPointWarpID) != 0) {
        temp_v0 = arg1->widget;
        temp_v0->flags |= 0x01000000;
    } else {
        temp_a0_2 = arg1->widget;
        temp_a0_2->flags &= 0xFEFFFFFF;
    }
    return 0;
}
